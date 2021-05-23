// Copyright (c) 2011-2020 The thecoffeecoins Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef thecoffeecoins_QT_thecoffeecoinsADDRESSVALIDATOR_H
#define thecoffeecoins_QT_thecoffeecoinsADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class thecoffeecoinsAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit thecoffeecoinsAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** thecoffeecoins address widget validator, checks for a valid thecoffeecoins address.
 */
class thecoffeecoinsAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit thecoffeecoinsAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // thecoffeecoins_QT_thecoffeecoinsADDRESSVALIDATOR_H
