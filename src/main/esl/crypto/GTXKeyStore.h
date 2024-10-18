#ifndef ESL_CRYPTO_GTXKEYSTORE_H_
#define ESL_CRYPTO_GTXKEYSTORE_H_

#include <esl/crypto/KeyStore.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace esl {
inline namespace v1_6 {
namespace crypto {

class GTXKeyStore : public KeyStore {
public:
	struct Settings {
		Settings() = default;
		Settings(const std::vector<std::pair<std::string, std::string>>& settings);
	};

	GTXKeyStore(const Settings& settings = Settings());

	static std::unique_ptr<KeyStore> create(const std::vector<std::pair<std::string, std::string>>& settings);
	static std::unique_ptr<KeyStore> createNative(const Settings& settings = Settings());

	void addCertificate(const std::string& hostname, const std::vector<unsigned char>& certificate) override;
	void addPrivateKey(const std::string& hostname, const std::vector<unsigned char>& key, const std::string& password) override;

private:
	std::unique_ptr<KeyStore> keyStore;
};

} /* namespace crypto */
} /* inline namespace v1_6 */
} /* namespace esl */

#endif /* ESL_CRYPTO_GTXKEYSTORE_H_ */
