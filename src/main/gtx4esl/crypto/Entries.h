#ifndef GTX4ESL_CRYPTO_ENTRIES_H_
#define GTX4ESL_CRYPTO_ENTRIES_H_

#include <gtx4esl/crypto/Entry.h>

#include <esl/object/Object.h>

#include <map>
#include <string>

namespace gtx4esl {
inline namespace v1_6 {
namespace crypto {

struct Entries : esl::object::Object {
	std::map<std::string, Entry> entryByHostname;
};

} /* namespace crypto */
} /* inline namespace v1_6 */
} /* namespace gtx4esl */

#endif /* GTX4ESL_CRYPTO_ENTRIES_H_ */
