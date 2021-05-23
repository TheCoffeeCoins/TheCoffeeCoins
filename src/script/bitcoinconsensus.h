// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The thecoffeecoins Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef thecoffeecoins_SCRIPT_thecoffeecoinsCONSENSUS_H
#define thecoffeecoins_SCRIPT_thecoffeecoinsCONSENSUS_H

#include <stdint.h>

#if defined(BUILD_thecoffeecoins_INTERNAL) && defined(HAVE_CONFIG_H)
#include <config/thecoffeecoins-config.h>
  #if defined(_WIN32)
    #if defined(HAVE_DLLEXPORT_ATTRIBUTE)
      #define EXPORT_SYMBOL __declspec(dllexport)
    #else
      #define EXPORT_SYMBOL
    #endif
  #elif defined(HAVE_DEFAULT_VISIBILITY_ATTRIBUTE)
    #define EXPORT_SYMBOL __attribute__ ((visibility ("default")))
  #endif
#elif defined(MSC_VER) && !defined(STATIC_LIBthecoffeecoinsCONSENSUS)
  #define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
  #define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define thecoffeecoinsCONSENSUS_API_VER 1

typedef enum thecoffeecoinsconsensus_error_t
{
    thecoffeecoinsconsensus_ERR_OK = 0,
    thecoffeecoinsconsensus_ERR_TX_INDEX,
    thecoffeecoinsconsensus_ERR_TX_SIZE_MISMATCH,
    thecoffeecoinsconsensus_ERR_TX_DESERIALIZE,
    thecoffeecoinsconsensus_ERR_AMOUNT_REQUIRED,
    thecoffeecoinsconsensus_ERR_INVALID_FLAGS,
} thecoffeecoinsconsensus_error;

/** Script verification flags */
enum
{
    thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_NONE                = 0,
    thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_P2SH                = (1U << 0), // evaluate P2SH (BIP16) subscripts
    thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_DERSIG              = (1U << 2), // enforce strict DER (BIP66) compliance
    thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY           = (1U << 4), // enforce NULLDUMMY (BIP147)
    thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9), // enable CHECKLOCKTIMEVERIFY (BIP65)
    thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10), // enable CHECKSEQUENCEVERIFY (BIP112)
    thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_WITNESS             = (1U << 11), // enable WITNESS (BIP141)
    thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_ALL                 = thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_P2SH | thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
                                                               thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY | thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY |
                                                               thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY | thecoffeecoinsconsensus_SCRIPT_FLAGS_VERIFY_WITNESS
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by
/// txTo correctly spends the scriptPubKey pointed to by scriptPubKey under
/// the additional constraints specified by flags.
/// If not nullptr, err will contain an error/success code for the operation
EXPORT_SYMBOL int thecoffeecoinsconsensus_verify_script(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen,
                                                 const unsigned char *txTo        , unsigned int txToLen,
                                                 unsigned int nIn, unsigned int flags, thecoffeecoinsconsensus_error* err);

EXPORT_SYMBOL int thecoffeecoinsconsensus_verify_script_with_amount(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen, int64_t amount,
                                    const unsigned char *txTo        , unsigned int txToLen,
                                    unsigned int nIn, unsigned int flags, thecoffeecoinsconsensus_error* err);

EXPORT_SYMBOL unsigned int thecoffeecoinsconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // thecoffeecoins_SCRIPT_thecoffeecoinsCONSENSUS_H
