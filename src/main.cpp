#include "../headers/io_utils.h"

int main() {
  std::string username = "user1";
  std::string password = "my_secure_password";

  // Create a User object and hash the password
  User user(username, password);

  std::cout << "Stored Hashed Password: " << user.getHashedPassword()
            << std::endl;

  // Verify the password
  if (user.verifyPassword("my_secure_password")) {
    std::cout << "Password verified successfully!" << std::endl;
  } else {
    std::cout << "Password verification failed!" << std::endl;
  }

  return 0;
}
