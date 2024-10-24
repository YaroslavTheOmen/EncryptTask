import os
from typing import List
from classes import Note
from note_utils import settings
import serialization
from cryptography.hazmat.primitives import hashes, padding
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.backends import default_backend


def encrypt_data(
    notes: List[Note], config: settings, filename: str, password: str
) -> bool:
    # Serialize data to a memory buffer
    from io import BytesIO

    buffer = BytesIO()
    serialization.save_data(notes, config, buffer)
    plaintext = buffer.getvalue()

    # Generate a random salt (16 bytes)
    salt = os.urandom(16)

    # Derive key using PBKDF2-HMAC-SHA1
    # Use 10000 iterations, key length 32 bytes (256 bits)
    kdf = PBKDF2HMAC(
        algorithm=hashes.SHA1(),
        length=32,
        salt=salt,
        iterations=10000,
        backend=default_backend(),
    )
    key = kdf.derive(password.encode("utf-8"))

    # Generate random IV (16 bytes)
    iv = os.urandom(16)

    # Encrypt the plaintext using AES-256-CBC
    cipher = Cipher(algorithms.AES(key), modes.CBC(iv), backend=default_backend())
    encryptor = cipher.encryptor()

    # Use PKCS7 padding
    padder = padding.PKCS7(128).padder()
    padded_plaintext = padder.update(plaintext) + padder.finalize()

    ciphertext = encryptor.update(padded_plaintext) + encryptor.finalize()

    # Write salt, IV, and ciphertext to file
    with open(filename, "wb") as f:
        f.write(salt)
        f.write(iv)
        f.write(ciphertext)

    return True


def decrypt_data(
    notes: List[Note], config: settings, filename: str, password: str
) -> bool:
    # Read salt, IV, and ciphertext from file
    with open(filename, "rb") as f:
        salt = f.read(16)
        if len(salt) != 16:
            raise ValueError("Failed to read salt.")
        iv = f.read(16)
        if len(iv) != 16:
            raise ValueError("Failed to read IV.")
        ciphertext = f.read()

    # Derive key using PBKDF2-HMAC-SHA1
    kdf = PBKDF2HMAC(
        algorithm=hashes.SHA1(),
        length=32,
        salt=salt,
        iterations=10000,
        backend=default_backend(),
    )
    key = kdf.derive(password.encode("utf-8"))

    # Decrypt the ciphertext using AES-256-CBC
    cipher = Cipher(algorithms.AES(key), modes.CBC(iv), backend=default_backend())
    decryptor = cipher.decryptor()

    try:
        padded_plaintext = decryptor.update(ciphertext) + decryptor.finalize()

        # Remove padding
        unpadder = padding.PKCS7(128).unpadder()
        plaintext = unpadder.update(padded_plaintext) + unpadder.finalize()
    except Exception as e:
        print("Decryption failed. Incorrect password or corrupted data.")
        return False

    # Deserialize data from plaintext
    from io import BytesIO

    buffer = BytesIO(plaintext)
    serialization.load_data(notes, config, buffer)

    return True
