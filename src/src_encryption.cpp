#include "../headers/encryption.h"
#include "../headers/serialization.h"
#include <fstream>
#include <iostream>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sstream>

bool encrypt_data(const std::vector<MyNote::note *> &notes,
                  const settings &config, const std::string &filename,
                  const std::string &password) {
  // Serialize data to a memory buffer
  std::ostringstream oss(std::ios::binary);
  save_data(notes, config, oss);

  std::string plaintext = oss.str();

  // Generate a random salt
  unsigned char salt[16];
  if (!RAND_bytes(salt, sizeof(salt))) {
    std::cerr << "Failed to generate salt." << std::endl;
    return false;
  }

  // Derive key using PBKDF2
  unsigned char key[32]; // 256 bits for AES-256
  if (!PKCS5_PBKDF2_HMAC_SHA1(password.c_str(), password.length(), salt,
                              sizeof(salt), 10000, sizeof(key), key)) {
    std::cerr << "Failed to derive key." << std::endl;
    return false;
  }

  // Generate random IV
  unsigned char iv[16]; // AES block size is 128 bits
  if (!RAND_bytes(iv, sizeof(iv))) {
    std::cerr << "Failed to generate IV." << std::endl;
    return false;
  }

  // Encrypt the plaintext
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    std::cerr << "Failed to create EVP_CIPHER_CTX." << std::endl;
    return false;
  }

  int len;
  int ciphertext_len;
  std::vector<unsigned char> ciphertext(plaintext.size() +
                                        EVP_MAX_BLOCK_LENGTH);

  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
    std::cerr << "Encryption initialization failed." << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  if (1 != EVP_EncryptUpdate(
               ctx, ciphertext.data(), &len,
               reinterpret_cast<const unsigned char *>(plaintext.data()),
               plaintext.size())) {
    std::cerr << "Encryption update failed." << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  ciphertext_len = len;

  if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) {
    std::cerr << "Encryption finalization failed." << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  ciphertext_len += len;

  EVP_CIPHER_CTX_free(ctx);

  // Write salt, IV, and ciphertext to file
  std::ofstream out(filename, std::ios::binary);
  if (!out) {
    std::cerr << "Failed to open file for writing." << std::endl;
    return false;
  }

  // Write salt
  out.write(reinterpret_cast<const char *>(salt), sizeof(salt));
  // Write IV
  out.write(reinterpret_cast<const char *>(iv), sizeof(iv));
  // Write ciphertext
  out.write(reinterpret_cast<const char *>(ciphertext.data()), ciphertext_len);

  out.close();

  return true;
}

bool decrypt_data(std::vector<MyNote::note *> &notes, settings &config,
                  const std::string &filename, const std::string &password) {
  // Read salt, IV, and ciphertext from file
  std::ifstream in(filename, std::ios::binary);
  if (!in) {
    std::cerr << "Failed to open file for reading." << std::endl;
    return false;
  }

  // Read salt
  unsigned char salt[16];
  in.read(reinterpret_cast<char *>(salt), sizeof(salt));
  if (in.gcount() != sizeof(salt)) {
    std::cerr << "Failed to read salt." << std::endl;
    return false;
  }

  // Read IV
  unsigned char iv[16];
  in.read(reinterpret_cast<char *>(iv), sizeof(iv));
  if (in.gcount() != sizeof(iv)) {
    std::cerr << "Failed to read IV." << std::endl;
    return false;
  }

  // Read ciphertext
  std::vector<unsigned char> ciphertext((std::istreambuf_iterator<char>(in)),
                                        std::istreambuf_iterator<char>());
  in.close();

  // Derive key using PBKDF2
  unsigned char key[32]; // 256 bits for AES-256

  if (!PKCS5_PBKDF2_HMAC_SHA1(password.c_str(), password.length(), salt,
                              sizeof(salt), 10000, sizeof(key), key)) {
    std::cerr << "Failed to derive key." << std::endl;
    return false;
  }

  // Decrypt the ciphertext
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    std::cerr << "Failed to create EVP_CIPHER_CTX." << std::endl;
    return false;
  }

  int len;
  int plaintext_len;
  std::vector<unsigned char> plaintext(ciphertext.size());

  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
    std::cerr << "Decryption initialization failed." << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(),
                             ciphertext.size())) {
    std::cerr << "Decryption update failed." << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  plaintext_len = len;

  if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) {
    std::cerr << "Decryption finalization failed. Incorrect password or "
                 "corrupted data."
              << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  plaintext_len += len;

  EVP_CIPHER_CTX_free(ctx);

  // Deserialize data from plaintext
  std::istringstream iss(
      std::string(reinterpret_cast<char *>(plaintext.data()), plaintext_len),
      std::ios::binary);
  load_data(notes, config, iss);

  return true;
}
