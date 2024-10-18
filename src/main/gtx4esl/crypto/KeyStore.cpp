#include <gtx4esl/crypto/KeyStore.h>
#include <gtx4esl/crypto/Entry.h>

#include <esl/Logger.h>
#include <esl/system/Stacktrace.h>

#include <memory>
#include <stdexcept>

namespace gtx4esl {
inline namespace v1_6 {
namespace crypto {

namespace {
esl::Logger logger("gtx4esl::crypto::KeyStore");
} /* anonymous namespace */

KeyStore::KeyStore(const esl::crypto::GTXKeyStore::Settings& aSettings)
: settings(aSettings)
{ }

void KeyStore::addCertificate(const std::string& hostname, const std::vector<unsigned char>& certificate) {
	gnutls_datum_t gnutls_datum;
	gnutls_datum.data = const_cast<unsigned char*>(&certificate[0]);
	gnutls_datum.size = static_cast<unsigned int>(certificate.size());

	Entry& entry = getEntries().entryByHostname[hostname];
	int rc = gnutls_pcert_import_x509_raw(&entry.pcrt, &gnutls_datum, GNUTLS_X509_FMT_PEM, 0);
	if(rc < 0) {
		logger.error << "Error installing certificate: " << gnutls_strerror (rc) << "\n";
		throw esl::system::Stacktrace::add(std::runtime_error("Error installing certificate: " + std::string(gnutls_strerror (rc))));
	}

	logger.info << "Successfully installed certificate hostname \"" << hostname << "\"\n";
}

void KeyStore::addPrivateKey(const std::string& hostname, const std::vector<unsigned char>& key, const std::string& password) {
	gnutls_datum_t gnutls_datum;
	gnutls_datum.data = const_cast<unsigned char*>(&key[0]);
	gnutls_datum.size = static_cast<unsigned int>(key.size());

	Entry& entry = getEntries().entryByHostname[hostname];
	gnutls_privkey_init(&entry.key);
	int rc = gnutls_privkey_import_x509_raw(entry.key, &gnutls_datum, GNUTLS_X509_FMT_PEM, password.empty() ? nullptr : password.c_str(), 0);
	if(rc < 0) {
		logger.error << "Error installing key: " << gnutls_strerror (rc) << "\n";
		throw esl::system::Stacktrace::add(std::runtime_error("Error installing key"));
	}

	logger.info << "Successfully installed private key for hostname \"" << hostname << "\"\n";
}
/*
void KeyStore::initializeContext(esl::object::Context& context) {
	if(!settings.registryId.empty()) {
		registry = context.findObject<esl::plugin::Registry>(settings.registryId);
		if(registry == nullptr) {
			throw esl::system::Stacktrace::add(std::runtime_error("Cannot find registry with id '" + settings.registryId + "'"));
		}
	}
}
*/
Entries& KeyStore::getEntries() {
	//esl::plugin::Registry& registryRef = registry ? *registry : esl::plugin::Registry::get();
	//Entries* entriesPtr = registryRef.findObject<Entries>();
	Entries* entriesPtr = esl::plugin::Registry::get().findObject<Entries>();
	if(!entriesPtr) {
		std::unique_ptr<Entries> entries(new Entries);
		entriesPtr = entries.get();
		esl::plugin::Registry::get().setObject(std::move(entries));
	}
	return *entriesPtr;
}

} /* namespace crypto */
} /* inline namespace v1_6 */
} /* namespace gtx4esl */
