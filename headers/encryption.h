#ifndef ENCRYPTION_H_
#define ENCRYPTION_H_

#include "classes.h"
#include "utils.h"
#include <string>
#include <vector>

// Encryption and decryption functions
bool encrypt_data(const std::vector<MyNote::note *> &notes,
                  const settings &config, const std::string &filename,
                  const std::string &password);

bool decrypt_data(std::vector<MyNote::note *> &notes, settings &config,
                  const std::string &filename, const std::string &password);

#endif // ENCRYPTION_H_
