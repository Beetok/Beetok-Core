// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2019 The Beetok Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "base58.h"
#include "libzerocoin/Params.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
	(0, uint256("0x8ab5bd3c2a0bd5f1755587c725b1cee1fbe9d48101993818b29519c374087052"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
	1562470070, // * UNIX timestamp of last checkpoint block
    0,     // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    200         // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x8ab5bd3c2a0bd5f1755587c725b1cee1fbe9d48101993818b29519c374087052"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1562470070,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x8ab5bd3c2a0bd5f1755587c725b1cee1fbe9d48101993818b29519c374087052"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1562470070,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus;
    bnTrustedModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

libzerocoin::ZerocoinParams* CChainParams::OldZerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus;
    bnTrustedModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x5a;
        pchMessageStart[1] = 0x5b;
        pchMessageStart[2] = 0x5c;
        pchMessageStart[3] = 0x5d;
        vAlertPubKey = ParseHex("04f58b24373ca630e24c1a3c10faddb12941bed8aba25439733c483c2921793565430a442c0016c8ef2f8e8437d1e3ce7905c506a69ed16d244f26f46ba3144fad");
        nDefaultPort = 1112;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Beetok starting difficulty is 1 / 2^12
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;

        nTargetTimespan = 1 * 60; // Beetok: 1 day
        nTargetSpacing = 1 * 60; // Beetok: 1 minute
        nLastPOWBlock = 200;
        nZerocoinStartHeight = INT_MAX;
        nMaturity = 20;
        nMasternodeCountDrift = 20;
        nMasternodeCollateralAmt = 2000;
        nMasternodeCollateralAmtNew = 5000;
        nMinStakeInput = 20 * COIN;
        nModifierUpdateBlock = 615800;
        nMaxMoneyOut = 82000000 * COIN;
        const char* pszTimestamp = "National Suicide Prevention Hotline 1-800-273-8255";

        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0459f0e955f4b6f1e65dce17d1bbf08218867a4b7b8d1de4675a1cf4d884eca8480ebdb847c42acff26a967177f35082666883cb98b2e9044c10bd842b37b79bbe") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1562470070;
        genesis.nBits = 504365040;
        genesis.nNonce = 652550;

        hashGenesisBlock = genesis.GetHash();
        //printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str());
        //printf("genesis.hashMerkleRoot = %s\n", genesis.hashMerkleRoot.ToString().c_str());
        assert(hashGenesisBlock == uint256("0x8ab5bd3c2a0bd5f1755587c725b1cee1fbe9d48101993818b29519c374087052"));
        assert(genesis.hashMerkleRoot == uint256("0x89c57ede80f9489547c381b653c4f70d5057a2570ce7b48c9e7fa7afbade012c"));

        vSeeds.push_back(CDNSSeedData("140.82.48.162", "140.82.48.162"));
        vSeeds.push_back(CDNSSeedData("8.9.36.49", "8.9.36.49"));
        vSeeds.push_back(CDNSSeedData("95.179.140.237", "95.179.140.237"));
        vSeeds.push_back(CDNSSeedData("136.244.101.5", "136.244.101.5"));
        vSeeds.push_back(CDNSSeedData("95.179.132.243", "95.179.132.243"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 25);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 43);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
		//  BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
		base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

		bech32_hrp = "ab";

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;

        nEnforceNewSporkKey = 1563580800; //07/20/2019 @ 12:00am (UTC)
        nRejectOldSporkKey = 1563580800; //07/20/2019 @ 12:00am (UTC)
		
		strSporkKey = "024e5c4ba539033389b20d7ae149d8b98338cc90b6db94b1fb79bf87ee99902721"; //This is owned by project owner.
        strSporkKeyOld = "03fd7122ee805bf78beac809cb4f5178214d0e83a7da09ebe167de7934df20e2bb"; //This is owned by TFinch for creation of project
        strObfuscationPoolDummyAddress = "BPaLkBnuXTLqB1VdC5RGUeHYq88X8YcWcm";
        nStartMasternodePayments = 1562470070;

        /** Zerocoin */
		/* This is not active */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                          "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                          "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                          "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                          "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                          "31438167899885040445364023527381951378636564391212010397122822120720357";

        nBudgetFeeConfirmations = 6; // Number of confirmations for the finalization fee
		
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};

static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x6a;
        pchMessageStart[1] = 0x7a;
        pchMessageStart[2] = 0x8a;
        pchMessageStart[3] = 0x9a;
        vAlertPubKey = ParseHex("04cb6591fe3441beb224cf5640fdc8c2143982899445c95a16b27918067826b68b822e4671be534be381aa57b600b25bf668833f5943cb851563d95ab8ead26d36");
        nDefaultPort = 55001;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Beetok: 1 day
        nTargetSpacing = 1 * 60;  // Beetok: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 1;
        nMasternodeCountDrift = 4;
        nMasternodeCollateralAmt = 1000;
        nMasternodeCollateralAmtNew = 5000;
        nMinStakeInput = 15 * COIN;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = INT_MAX;
        nZerocoinLastOldParams = INT_MAX;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1562470070;
        genesis.nNonce = 652550;

		//printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str());
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x8ab5bd3c2a0bd5f1755587c725b1cee1fbe9d48101993818b29519c374087052"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 85); // Testnet beetok addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19); // Testnet beetok script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);    // Testnet private keys start with '9' or 'c' (Bitcoin defaults)        // Testnet beetok BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet beetok BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet beetok BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        bech32_hrp = "tv";

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 2;
        strSporkKey = "03fd7122ee805bf78beac809cb4f5178214d0e83a7da09ebe167de7934df20e2bb";

        strObfuscationPoolDummyAddress = "BPaLkBnuXTLqB1VdC5RGUeHYq88X8YcWcm";
        nStartMasternodePayments = 1562470070;
	}
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xe1;
        pchMessageStart[1] = 0xef;
        pchMessageStart[2] = 0xee;
        pchMessageStart[3] = 0xec;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Beetok: 1 day
        nTargetSpacing = 1 * 60;        // Beetok: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1562470070;
        genesis.nBits = 504365040;
        genesis.nNonce = 652550;
        nMaturity = 0;
        nLastPOWBlock = 999999999; // PoS complicates Regtest because of timing issues
        nZerocoinLastOldParams = 499;
        nZerocoinStartHeight = 100;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 39793;
        assert(hashGenesisBlock == uint256("0x8ab5bd3c2a0bd5f1755587c725b1cee1fbe9d48101993818b29519c374087052"));


        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        nRequiredAccumulation = 1;

        // {
        //     "PrivateKey": "923EhWh2bJHynX6d4Tqt2Q75bhTDCT1b4kff3qzDKDZHZ6pkQs7",
        //     "PublicKey": "04866dc02c998b7e1ab16fe14e0d86554595da90c36acb706a4d763b58ed0edb1f82c87e3ced065c5b299b26e12496956b9e5f9f19aa008b5c46229b15477c875a"
        // }
        strSporkKey = "04cded1204a57acd6280c8499b7a2df052609dbf96546453984d632204d651d72a37013edc9d115e5a385e100eb7e867923fdd0bb7d9dc31aa1eb9d59b00c76697";
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 39791;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
