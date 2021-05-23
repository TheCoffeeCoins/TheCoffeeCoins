// Copyright (c) 2018 The thecoffeecoins Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef thecoffeecoins_COMPAT_STDIN_H
#define thecoffeecoins_COMPAT_STDIN_H

struct NoechoInst {
    NoechoInst();
    ~NoechoInst();
};

#define NO_STDIN_ECHO() NoechoInst _no_echo

bool StdinTerminal();
bool StdinReady();

#endif // thecoffeecoins_COMPAT_STDIN_H
