#include <gtx4esl/crypto/KeyStore.h>

#include <esl/crypto/GTXKeyStore.h>
#include <esl/system/Stacktrace.h>

#include <stdexcept>

namespace esl {
inline namespace v1_6 {
namespace crypto {

GTXKeyStore::Settings::Settings(const std::vector<std::pair<std::string, std::string>>& settings) {
    for(const auto& setting : settings) {
		throw esl::system::Stacktrace::add(std::runtime_error("unknown attribute '\"" + setting.first + "\"'."));
    }
}

GTXKeyStore::GTXKeyStore(const Settings& settings)
: keyStore(new gtx4esl::crypto::KeyStore(settings))
{ }

std::unique_ptr<KeyStore> GTXKeyStore::create(const std::vector<std::pair<std::string, std::string>>& settings) {
	return std::unique_ptr<KeyStore>(new GTXKeyStore(Settings(settings)));
}

std::unique_ptr<KeyStore> GTXKeyStore::createNative(const Settings& settings) {
	return std::unique_ptr<KeyStore>(new gtx4esl::crypto::KeyStore(settings));
}

void GTXKeyStore::addCertificate(const std::string& hostname, const std::vector<unsigned char>& certificate) {
	keyStore->addCertificate(hostname, certificate);
}

void GTXKeyStore::addPrivateKey(const std::string& hostname, const std::vector<unsigned char>& key, const std::string& password) {
	keyStore->addPrivateKey(hostname, key, password);
}

} /* namespace crypto */
} /* inline namespace v1_6 */
} /* namespace esl */
