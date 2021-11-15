/*!
 * signet.c - signet for mako
 * Copyright (c) 2021, Christopher Jeffrey (MIT License).
 * https://github.com/chjj/mako
 */

#include <stddef.h>
#include <stdint.h>
#include <mako/network.h>
#include "internal.h"

static const char *signet_seeds[] = {
  "seed.signet.bitcoin.sprovoost.nl.",
  "178.128.221.177"
};

static const btc_checkpoint_t signet_checkpoints[] = {
  {
    10000,
    {
      0x92, 0x16, 0x1f, 0xd3, 0x98, 0x96, 0x57, 0x03,
      0x74, 0xf8, 0x97, 0x0c, 0x9d, 0x6e, 0x98, 0xfe,
      0xca, 0xec, 0x15, 0x51, 0x00, 0x23, 0x9f, 0xfe,
      0x51, 0xac, 0x99, 0xe6, 0xad, 0x00, 0x00, 0x00
    }
  },
  {
    20000,
    {
      0xd1, 0xbb, 0x9e, 0xfa, 0x68, 0x0a, 0x8b, 0xbf,
      0x8d, 0xdd, 0xb5, 0x20, 0x14, 0xe7, 0xef, 0x56,
      0x3a, 0xa9, 0x8b, 0x7f, 0x12, 0x7e, 0xbf, 0xdd,
      0x0e, 0x96, 0x68, 0x63, 0xd8, 0x00, 0x00, 0x00
    }
  },
  {
    30000,
    {
      0xf0, 0x29, 0x67, 0xd7, 0x4b, 0x3d, 0xe9, 0x9a,
      0xff, 0xd8, 0x19, 0x98, 0xca, 0xff, 0xad, 0x0d,
      0x63, 0xfc, 0xb5, 0xca, 0x64, 0xd7, 0x19, 0xef,
      0x1c, 0xdf, 0x9d, 0x62, 0x18, 0x00, 0x00, 0x00
    }
  },
  {
    40000,
    {
      0x3c, 0x79, 0x23, 0x57, 0x60, 0x5c, 0xe0, 0x8b,
      0xc1, 0x87, 0xee, 0x5a, 0xb8, 0xa9, 0xb8, 0x5e,
      0xce, 0xe1, 0x2c, 0x9c, 0x17, 0x52, 0xbd, 0x36,
      0x68, 0xfe, 0xdd, 0x86, 0x40, 0x01, 0x00, 0x00
    }
  },
  {
    50000,
    {
      0x01, 0xcb, 0x97, 0xe3, 0xe4, 0x0d, 0xe8, 0xb6,
      0xc1, 0x95, 0xfd, 0xbf, 0xef, 0xd6, 0x49, 0x00,
      0xe9, 0x40, 0x81, 0x2e, 0xa9, 0x85, 0xce, 0xbd,
      0xa4, 0x9e, 0x56, 0x3b, 0xf4, 0x00, 0x00, 0x00
    }
  },
  {
    60000,
    {
      0xf1, 0x01, 0xc8, 0xa4, 0x0e, 0xad, 0x82, 0xf4,
      0x22, 0x2b, 0xb8, 0x97, 0xc1, 0xce, 0x3d, 0x76,
      0xf9, 0x35, 0xae, 0xf7, 0xb3, 0xac, 0x32, 0xe2,
      0x4e, 0xa7, 0x66, 0xab, 0x30, 0x01, 0x00, 0x00
    }
  }
};

static const uint8_t signet_genesis[] = {
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x3b, 0xa3, 0xed, 0xfd,
  0x7a, 0x7b, 0x12, 0xb2, 0x7a, 0xc7, 0x2c, 0x3e,
  0x67, 0x76, 0x8f, 0x61, 0x7f, 0xc8, 0x1b, 0xc3,
  0x88, 0x8a, 0x51, 0x32, 0x3a, 0x9f, 0xb8, 0xaa,
  0x4b, 0x1e, 0x5e, 0x4a, 0x00, 0x8f, 0x4d, 0x5f,
  0xae, 0x77, 0x03, 0x1e, 0x8a, 0xd2, 0x22, 0x03,
  0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0xff, 0x4d, 0x04, 0xff, 0xff, 0x00, 0x1d,
  0x01, 0x04, 0x45, 0x54, 0x68, 0x65, 0x20, 0x54,
  0x69, 0x6d, 0x65, 0x73, 0x20, 0x30, 0x33, 0x2f,
  0x4a, 0x61, 0x6e, 0x2f, 0x32, 0x30, 0x30, 0x39,
  0x20, 0x43, 0x68, 0x61, 0x6e, 0x63, 0x65, 0x6c,
  0x6c, 0x6f, 0x72, 0x20, 0x6f, 0x6e, 0x20, 0x62,
  0x72, 0x69, 0x6e, 0x6b, 0x20, 0x6f, 0x66, 0x20,
  0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x20, 0x62,
  0x61, 0x69, 0x6c, 0x6f, 0x75, 0x74, 0x20, 0x66,
  0x6f, 0x72, 0x20, 0x62, 0x61, 0x6e, 0x6b, 0x73,
  0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0xf2, 0x05,
  0x2a, 0x01, 0x00, 0x00, 0x00, 0x43, 0x41, 0x04,
  0x67, 0x8a, 0xfd, 0xb0, 0xfe, 0x55, 0x48, 0x27,
  0x19, 0x67, 0xf1, 0xa6, 0x71, 0x30, 0xb7, 0x10,
  0x5c, 0xd6, 0xa8, 0x28, 0xe0, 0x39, 0x09, 0xa6,
  0x79, 0x62, 0xe0, 0xea, 0x1f, 0x61, 0xde, 0xb6,
  0x49, 0xf6, 0xbc, 0x3f, 0x4c, 0xef, 0x38, 0xc4,
  0xf3, 0x55, 0x04, 0xe5, 0x1e, 0xc1, 0x12, 0xde,
  0x5c, 0x38, 0x4d, 0xf7, 0xba, 0x0b, 0x8d, 0x57,
  0x8a, 0x4c, 0x70, 0x2b, 0x6b, 0xf1, 0x1d, 0x5f,
  0xac, 0x00, 0x00, 0x00, 0x00
};

static const btc_deployment_t signet_deployments[] = {
  {
    /* .name = */ "testdummy",
    /* .bit = */ 28,
    /* .start_time = */ 1199145601, /* January 1, 2008 */
    /* .timeout = */ 1230767999, /* December 31, 2008 */
    /* .threshold = */ -1,
    /* .window = */ -1,
    /* .required = */ 0,
    /* .force = */ 1
  }
};

static uint8_t signet_challenge[] = {
  0x51, 0x21, 0x03, 0xad, 0x5e, 0x0e, 0xda, 0xd1,
  0x8c, 0xb1, 0xf0, 0xfc, 0x0d, 0x28, 0xa3, 0xd4,
  0xf1, 0xf3, 0xe4, 0x45, 0x64, 0x03, 0x37, 0x48,
  0x9a, 0xbb, 0x10, 0x40, 0x4f, 0x2d, 0x1e, 0x08,
  0x6b, 0xe4, 0x30, 0x21, 0x03, 0x59, 0xef, 0x50,
  0x21, 0x96, 0x4f, 0xe2, 0x2d, 0x6f, 0x8e, 0x05,
  0xb2, 0x46, 0x3c, 0x95, 0x40, 0xce, 0x96, 0x88,
  0x3f, 0xe3, 0xb2, 0x78, 0x76, 0x0f, 0x04, 0x8f,
  0x51, 0x89, 0xf2, 0xe6, 0xc4, 0x52, 0xae
};

static const btc_network_t signet = {
  /* .type = */ BTC_NETWORK_SIGNET,
  /* .name = */ "signet",
  /* .seeds = */ {
    /* .items = */ signet_seeds,
    /* .length = */ lengthof(signet_seeds)
  },
  /* .magic = */ 0x40cf030a,
  /* .port = */ 38333,
  /* .checkpoints = */ {
    /* .items = */ signet_checkpoints,
    /* .length = */ lengthof(signet_checkpoints)
  },
  /* .last_checkpoint */ 60000,
  /* .halving_interval = */ 210000,
  /* .genesis = */ {
    /* .hash = */ {
      0xf6, 0x1e, 0xee, 0x3b, 0x63, 0xa3, 0x80, 0xa4,
      0x77, 0xa0, 0x63, 0xaf, 0x32, 0xb2, 0xbb, 0xc9,
      0x7c, 0x9f, 0xf9, 0xf0, 0x1f, 0x2c, 0x42, 0x25,
      0xe9, 0x73, 0x98, 0x81, 0x08, 0x00, 0x00, 0x00
    },
    /* .header = */ {
      /* .version = */ 1,
      /* .prev_block = */ {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      },
      /* .merkle_root = */ {
        0x3b, 0xa3, 0xed, 0xfd, 0x7a, 0x7b, 0x12, 0xb2,
        0x7a, 0xc7, 0x2c, 0x3e, 0x67, 0x76, 0x8f, 0x61,
        0x7f, 0xc8, 0x1b, 0xc3, 0x88, 0x8a, 0x51, 0x32,
        0x3a, 0x9f, 0xb8, 0xaa, 0x4b, 0x1e, 0x5e, 0x4a
      },
      /* .time = */ 1598918400,
      /* .bits = */ 503543726,
      /* .nonce = */ 52613770
    },
    /* .data = */ signet_genesis,
    /* .length = */ sizeof(signet_genesis)
  },
  /* .pow = */ {
    /* .limit = */ {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0xae, 0x77, 0x03, 0x00, 0x00
    },
    /* .bits = */ 503543726,
    /* .chainwork = */ {
      0x9b, 0xa8, 0x40, 0x3e, 0xa9, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    },
    /* .target_timespan = */ 14 * 24 * 60 * 60,
    /* .target_spacing = */ 10 * 60,
    /* .retarget_interval = */ 2016,
    /* .target_reset = */ 0,
    /* .no_retargeting = */ 0
  },
  /* .block = */ {
    /* .prune_after_height = */ 1000,
    /* .keep_blocks = */ 10000,
    /* .max_tip_age = */ 24 * 60 * 60,
    /* .slow_height = */ 0
  },
  /* .softforks = */ {
    /* .bip30 = */ {
      /* .items = */ NULL,
      /* .length = */ 0
    },
    /* .bip34 = */ {
      1,
      {
        0x53, 0x3b, 0x53, 0xde, 0xd9, 0xbf, 0xf4, 0xad,
        0xc9, 0x41, 0x01, 0xd3, 0x24, 0x00, 0xa1, 0x44,
        0xc5, 0x4e, 0xdc, 0x5e, 0xd4, 0x92, 0xa3, 0xb2,
        0x6c, 0x63, 0xb2, 0xd6, 0x86, 0x00, 0x00, 0x00
      }
    },
    /* .bip65 = */ {
      1,
      {
        0x53, 0x3b, 0x53, 0xde, 0xd9, 0xbf, 0xf4, 0xad,
        0xc9, 0x41, 0x01, 0xd3, 0x24, 0x00, 0xa1, 0x44,
        0xc5, 0x4e, 0xdc, 0x5e, 0xd4, 0x92, 0xa3, 0xb2,
        0x6c, 0x63, 0xb2, 0xd6, 0x86, 0x00, 0x00, 0x00
      }
    },
    /* .bip66 = */ {
      1,
      {
        0x53, 0x3b, 0x53, 0xde, 0xd9, 0xbf, 0xf4, 0xad,
        0xc9, 0x41, 0x01, 0xd3, 0x24, 0x00, 0xa1, 0x44,
        0xc5, 0x4e, 0xdc, 0x5e, 0xd4, 0x92, 0xa3, 0xb2,
        0x6c, 0x63, 0xb2, 0xd6, 0x86, 0x00, 0x00, 0x00
      }
    },
    /* .csv = */ {
      1,
      {
        0x53, 0x3b, 0x53, 0xde, 0xd9, 0xbf, 0xf4, 0xad,
        0xc9, 0x41, 0x01, 0xd3, 0x24, 0x00, 0xa1, 0x44,
        0xc5, 0x4e, 0xdc, 0x5e, 0xd4, 0x92, 0xa3, 0xb2,
        0x6c, 0x63, 0xb2, 0xd6, 0x86, 0x00, 0x00, 0x00
      }
    },
    /* .segwit = */ {
      1,
      {
        0x53, 0x3b, 0x53, 0xde, 0xd9, 0xbf, 0xf4, 0xad,
        0xc9, 0x41, 0x01, 0xd3, 0x24, 0x00, 0xa1, 0x44,
        0xc5, 0x4e, 0xdc, 0x5e, 0xd4, 0x92, 0xa3, 0xb2,
        0x6c, 0x63, 0xb2, 0xd6, 0x86, 0x00, 0x00, 0x00
      }
    }
  },
  /* .activation_threshold = */ 1815, /* 90% of 2016 */
  /* .miner_window = */ 2016, /* pow.target_timespan / pow.target_spacing */
  /* .deployments = */ {
    /* .items = */ signet_deployments,
    /* .length = */ lengthof(signet_deployments)
  },
  /* .challenge = */ {
    /* .data = */ signet_challenge,
    /* .alloc = */ 0,
    /* .length = */ sizeof(signet_challenge),
    /* ._refs = */ 0
  },
  /* .key = */ {
    /* .privkey = */ 0xef,
    /* .xpubkey = */ {
      0x043587cf, /* tpub (p2pkh or p2sh) */
      0x044a5262, /* upub (nested p2wpkh) */
      0x045f1cf6, /* vpub (native p2wpkh) */
      0x024289ef, /* Upub (nested p2wsh) */
      0x02575483  /* Vpub (native p2wsh) */
    },
    /* .xprvkey = */ {
      0x04358394, /* tprv (p2pkh or p2sh) */
      0x044a4e28, /* uprv (nested p2wpkh) */
      0x045f18bc, /* vprv (native p2wpkh) */
      0x024285b5, /* Uprv (nested p2wsh) */
      0x02575048  /* Vprv (native p2wsh) */
    },
    /* .coin_type = */ 1
  },
  /* .address = */ {
    /* .p2pkh = */ 0x6f,
    /* .p2sh = */ 0xc4,
    /* .bech32 = */ "tb"
  },
  /* .require_standard = */ 1,
  /* .rpc_port = */ 38332,
  /* .min_relay = */ 1000,
  /* .fee_rate = */ 20000,
  /* .max_fee_rate = */ 60000,
  /* .self_connect = */ 0,
  /* .request_mempool = */ 0
};

const btc_network_t *btc_signet = &signet;
