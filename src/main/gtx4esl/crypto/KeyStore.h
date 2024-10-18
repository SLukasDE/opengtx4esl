#ifndef GTX4ESL_CRYPTO_KEYSTORE_H_
#define GTX4ESL_CRYPTO_KEYSTORE_H_

#include <gtx4esl/crypto/Entries.h>

#include <esl/crypto/GTXKeyStore.h>
#include <esl/crypto/KeyStore.h>
#include <esl/plugin/Registry.h>

namespace gtx4esl {
inline namespace v1_6 {
namespace crypto {

class KeyStore : public esl::crypto::KeyStore {
public:
	KeyStore(const esl::crypto::GTXKeyStore::Settings& settings);

	void addCertificate(const std::string& hostname, const std::vector<unsigned char>& certificate) override;
	void addPrivateKey(const std::string& hostname, const std::vector<unsigned char>& key, const std::string& password) override;

	Entries& getEntries();

private:
	esl::crypto::GTXKeyStore::Settings settings;
};

} /* namespace crypto */
} /* inline namespace v1_6 */
} /* namespace gtx4esl */

#endif /* GTX4ESL_CRYPTO_KEYSTORE_H_ */
