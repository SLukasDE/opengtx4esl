#ifndef GTX4ESL_CRYPTO_ENTRY_H_
#define GTX4ESL_CRYPTO_ENTRY_H_

#include <gnutls/gnutls.h>
#include <gnutls/abstract.h>

namespace gtx4esl {
inline namespace v1_6 {
namespace crypto {

struct Entry {
	gnutls_pcert_st pcrt;
	gnutls_privkey_t key;
};

} /* namespace crypto */
} /* inline namespace v1_6 */
} /* namespace gtx4esl */

#endif /* GTX4ESL_CRYPTO_ENTRY_H_ */
