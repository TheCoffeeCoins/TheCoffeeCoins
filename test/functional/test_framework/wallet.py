#!/usr/bin/env python3
# Copyright (c) 2020 The thecoffeecoins Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""A limited-functionality wallet, which may replace a real wallet in tests"""

from decimal import Decimal
from test_framework.address import ADDRESS_BCRT1_P2WSH_OP_TRUE
from test_framework.messages import (
    COIN,
    COutPoint,
    CTransaction,
    CTxIn,
    CTxInWitness,
    CTxOut,
)
from test_framework.script import (
    CScript,
    OP_TRUE,
    OP_NOP,
)
from test_framework.util import (
    assert_equal,
    hex_str_to_bytes,
    satoshi_round,
)


class MiniWallet:
    def __init__(self, test_node, *, raw_script=False):
        self._test_node = test_node
        self._utxos = []
        if raw_script:
            self._address = None
            self._scriptPubKey = bytes(CScript([OP_TRUE]))
        else:
            self._address = ADDRESS_BCRT1_P2WSH_OP_TRUE
            self._scriptPubKey = hex_str_to_bytes(self._test_node.validateaddress(self._address)['scriptPubKey'])

    def scan_blocks(self, *, start=1, num):
        """Scan the blocks for self._address outputs and add them to self._utxos"""
        for i in range(start, start + num):
            block = self._test_node.getblock(blockhash=self._test_node.getblockhash(i), verbosity=2)
            for tx in block['tx']:
                self.scan_tx(tx)

    def scan_tx(self, tx):
        """Scan the tx for self._scriptPubKey outputs and add them to self._utxos"""
        for out in tx['vout']:
            if out['scriptPubKey']['hex'] == self._scriptPubKey.hex():
                self._utxos.append({'txid': tx['txid'], 'vout': out['n'], 'value': out['value']})

    def generate(self, num_blocks):
        """Generate blocks with coinbase outputs to the internal address, and append the outputs to the internal list"""
        blocks = self._test_node.generatetodescriptor(num_blocks, f'raw({self._scriptPubKey.hex()})')
        for b in blocks:
            cb_tx = self._test_node.getblock(blockhash=b, verbosity=2)['tx'][0]
            self._utxos.append({'txid': cb_tx['txid'], 'vout': 0, 'value': cb_tx['vout'][0]['value']})
        return blocks

    def get_address(self):
        return self._address

    def get_utxo(self, *, txid='', mark_as_spent=True):
        """
        Returns a utxo and marks it as spent (pops it from the internal list)

        Args:
        txid (string), optional: get the first utxo we find from a specific transaction

        Note: Can be used to get the change output immediately after a send_self_transfer
        """
        index = -1  # by default the last utxo
        if txid:
            utxo = next(filter(lambda utxo: txid == utxo['txid'], self._utxos))
            index = self._utxos.index(utxo)
        if mark_as_spent:
            return self._utxos.pop(index)
        else:
            return self._utxos[index]

    def send_self_transfer(self, *, fee_rate=Decimal("0.003"), from_node, utxo_to_spend=None):
        """Create and send a tx with the specified fee_rate. Fee may be exact or at most one satoshi higher than needed."""
        tx = self.create_self_transfer(fee_rate=fee_rate, from_node=from_node, utxo_to_spend=utxo_to_spend)
        self.sendrawtransaction(from_node=from_node, tx_hex=tx['hex'])
        return tx

    def create_self_transfer(self, *, fee_rate=Decimal("0.003"), from_node, utxo_to_spend=None, mempool_valid=True):
        """Create and return a tx with the specified fee_rate. Fee may be exact or at most one satoshi higher than needed."""
        self._utxos = sorted(self._utxos, key=lambda k: k['value'])
        utxo_to_spend = utxo_to_spend or self._utxos.pop()  # Pick the largest utxo (if none provided) and hope it covers the fee
        vsize = Decimal(96)
        send_value = satoshi_round(utxo_to_spend['value'] - fee_rate * (vsize / 1000))
        fee = utxo_to_spend['value'] - send_value
        assert send_value > 0

        tx = CTransaction()
        tx.vin = [CTxIn(COutPoint(int(utxo_to_spend['txid'], 16), utxo_to_spend['vout']))]
        tx.vout = [CTxOut(int(send_value * COIN), self._scriptPubKey)]
        if not self._address:
            # raw script
            tx.vin[0].scriptSig = CScript([OP_NOP] * 35)  # pad to identical size
        else:
            tx.wit.vtxinwit = [CTxInWitness()]
            tx.wit.vtxinwit[0].scriptWitness.stack = [CScript([OP_TRUE])]
        tx_hex = tx.serialize().hex()

        tx_info = from_node.testmempoolaccept([tx_hex])[0]
        assert_equal(mempool_valid, tx_info['allowed'])
        if mempool_valid:
            assert_equal(tx_info['vsize'], vsize)
            assert_equal(tx_info['fees']['base'], fee)
        return {'txid': tx_info['txid'], 'wtxid': tx_info['wtxid'], 'hex': tx_hex, 'tx': tx}

    def sendrawtransaction(self, *, from_node, tx_hex):
        from_node.sendrawtransaction(tx_hex)
        self.scan_tx(from_node.decoderawtransaction(tx_hex))
