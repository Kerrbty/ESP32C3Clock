#include <pgmspace.h>
// 24*24 
const uint16_t gImage_rain[] = {
    0xffdf, 0xffff, 0xe73c, 0xf77d, 0xffff, 0xf7be, 0xef3c, 0xffde, 0xffff, 0xef7d, 0xef5d, 0xffff, 0xffff, 0xef5c, 0xef5c, 0xffff, 0xf7df, 0xe75d, 0xf7df, 0xffff, 0xf79e, 0xe73c, 0xffde, 0xffff, 
    0xffff, 0xffdf, 0xef5d, 0xffff, 0xffdf, 0xf7be, 0xf79e, 0xf7be, 0xffff, 0xffdf, 0xffdf, 0xffdf, 0xf7be, 0xf7be, 0xffde, 0xf7be, 0xf7df, 0xef7d, 0xf79e, 0xf7be, 0xf7be, 0xef7d, 0xffbe, 0xffff, 
    0xef5d, 0xef7d, 0xf7be, 0xffff, 0xe73d, 0xef7e, 0xffff, 0xffff, 0xe77e, 0xef7e, 0xef7e, 0xef9e, 0xf7df, 0xffff, 0xf7bf, 0xef5d, 0xf79e, 0xffde, 0xffbe, 0xef5d, 0xffff, 0xffff, 0xf7be, 0xef3c, 
    0xf79d, 0xffdf, 0xffff, 0xffdf, 0xe75d, 0xf7df, 0xffff, 0xef7e, 0xb618, 0xd6fc, 0xd6dc, 0xc659, 0xd6fc, 0xffff, 0xffff, 0xf7df, 0xffde, 0xffff, 0xffbe, 0xef5c, 0xffdf, 0xffde, 0xf79e, 0xef7d, 
    0xffff, 0xf7be, 0xe73d, 0xef7e, 0xffff, 0xffff, 0xdf3d, 0xbe39, 0xdf7e, 0xefdf, 0xefff, 0xdf3e, 0xce9b, 0xd6dc, 0xe77e, 0xef9f, 0xf7be, 0xf79d, 0xf7be, 0xf79e, 0xf7be, 0xef5d, 0xf7be, 0xffff, 
    0xffbe, 0xf7be, 0xf7be, 0xffff, 0xffff, 0xef9f, 0xc69b, 0xc69b, 0xefff, 0xcefd, 0xdf5f, 0xe7bf, 0xc67b, 0xadd8, 0xbe5a, 0xc69b, 0xce7a, 0xdf3d, 0xffff, 0xffff, 0xf7bf, 0xef7d, 0xf7be, 0xf7be, 
    0xe73c, 0xf79e, 0xffff, 0xf7bf, 0xc67a, 0xbe5a, 0xbe7a, 0xc6bc, 0xc6dd, 0xbe9b, 0xc69c, 0xcedd, 0xcf1d, 0xdf7f, 0xe7bf, 0xdf7f, 0xb63a, 0xcedc, 0xe79f, 0xef9f, 0xf7ff, 0xffdf, 0xffbe, 0xe71b, 
    0xffde, 0xf7be, 0xffff, 0xdf1c, 0xadd7, 0xdf3d, 0xefff, 0xd73e, 0xae3a, 0xbe7c, 0xa5d9, 0xa5b9, 0xcedd, 0xcefd, 0xb63a, 0xbe5b, 0xcf5f, 0xae3b, 0xadf9, 0xcedc, 0xf7ff, 0xf7be, 0xf79d, 0xffde, 
    0xf7ff, 0xffff, 0xf77e, 0xbe19, 0xc6dc, 0xae7b, 0x95da, 0xa63c, 0x9dd9, 0xadfa, 0xc6dd, 0xbe7b, 0x9db9, 0x9dba, 0x95db, 0x9e1d, 0xadba, 0xcefe, 0xbebc, 0xa5f9, 0xb65a, 0xefdf, 0xf77f, 0xffdf, 
    0xf7bd, 0xf7be, 0xf7bf, 0xadf9, 0x8d78, 0x8db9, 0x95b9, 0x7cb5, 0x8d36, 0xdf7f, 0xf7ff, 0xe79f, 0xb61a, 0x9537, 0x9d99, 0x8d58, 0x8d99, 0x9599, 0xa5da, 0xae1a, 0xadb9, 0xe77f, 0xffff, 0xef7e, 
    0xf75b, 0xf77d, 0xffff, 0x9d99, 0x5c55, 0x7538, 0x7cf6, 0x9db9, 0xcefd, 0xd73e, 0xc69b, 0xd6dc, 0xd71d, 0xdf3e, 0xdf7f, 0xadf9, 0x6cb5, 0x74f6, 0x8d79, 0x7cf6, 0x9537, 0xef9f, 0xef9e, 0xef5c, 
    0xfffd, 0xffff, 0xf79f, 0xc6bd, 0x6474, 0x4391, 0x9e3c, 0xcf7f, 0xcf3f, 0xae5b, 0xadfa, 0xbe5b, 0xc69c, 0xc6bc, 0xc6bc, 0xc6bc, 0x9dfb, 0x8579, 0x5c74, 0x4bf2, 0x7d15, 0xe7bf, 0xffff, 0xffde, 
    0xffff, 0xffff, 0xf77e, 0xffff, 0xbe7a, 0x6cb4, 0x8dba, 0xa65e, 0x95ba, 0x95da, 0x9e1b, 0x95da, 0x95ba, 0x9599, 0x8d78, 0xae5b, 0xbeff, 0xa65c, 0x5413, 0x6cf5, 0xc75e, 0xefff, 0xffde, 0xffde, 
    0xdf7e, 0xe75d, 0xffdf, 0xffde, 0xf7ff, 0xdfdf, 0x7cf6, 0x6476, 0x74f7, 0x7518, 0x7518, 0x7539, 0x7539, 0x857a, 0x859a, 0x6c96, 0x6cd6, 0x6cb5, 0xae5b, 0xefdf, 0xf7ff, 0xffff, 0xf79d, 0xe73b, 
    0xe79e, 0xf7bf, 0xffbf, 0xffff, 0xefbe, 0xf7ff, 0xbe9b, 0x6bd2, 0x5c13, 0x5c34, 0x53f3, 0x5c56, 0x5456, 0x4bf4, 0x5c35, 0x5434, 0x53d1, 0x84f6, 0xe79f, 0xffff, 0xffdf, 0xffff, 0xef5d, 0xf7df, 
    0xffff, 0xffdf, 0xef5d, 0xef5e, 0xcebb, 0xe7bf, 0xefdf, 0xe73d, 0xdfdf, 0xae7c, 0x53f3, 0x53f4, 0x7d9a, 0x5435, 0x43b2, 0xa69e, 0xcefe, 0xdf7f, 0xd6fc, 0xe79d, 0xf7bd, 0xe73c, 0xffbf, 0xffdf, 
    0xffbd, 0xffde, 0xefbf, 0xcefd, 0xbe7b, 0xdf5e, 0xffff, 0xffff, 0xffdf, 0xffff, 0xef7c, 0xe77c, 0xffde, 0xffbe, 0xf77d, 0xffde, 0xf7ff, 0xdf5e, 0xbe5a, 0xdf3d, 0xffff, 0xf7bf, 0xf7bf, 0xffff, 
    0xef5c, 0xef9d, 0xffff, 0xe79f, 0xcedc, 0xe79e, 0xffff, 0xef5d, 0xd71d, 0xffff, 0xffff, 0xe79e, 0xdf7e, 0xe79f, 0xef9e, 0xf7df, 0xef9f, 0xefbf, 0xd6dc, 0xdf1c, 0xffff, 0xffff, 0xef7d, 0xe71b, 
    0xf7de, 0xf7bf, 0xef9e, 0xefbf, 0xf7df, 0xefbf, 0xe75d, 0xdf1c, 0xa65b, 0xd75e, 0xf7df, 0xefdf, 0xc71e, 0xa63a, 0xb67b, 0xe7bf, 0xf7ff, 0xef9e, 0xef7e, 0xffff, 0xffdf, 0xf79e, 0xffff, 0xf79e, 
    0xffff, 0xffff, 0xef5d, 0xef7d, 0xffff, 0xf7df, 0xd6fc, 0xd71d, 0x95fa, 0xbe9b, 0xef5e, 0xf7df, 0xcf5f, 0x9e1b, 0xb67b, 0xf7ff, 0xf7df, 0xef5d, 0xf79e, 0xffff, 0xef7d, 0xe71c, 0xffde, 0xffff, 
    0xe77e, 0xf7bf, 0xffff, 0xffbe, 0xf77d, 0xffff, 0xf7df, 0xd6fc, 0xae7a, 0xe79e, 0xffff, 0xf79e, 0xdf7e, 0xd79f, 0xe7bf, 0xffde, 0xf77d, 0xffff, 0xffdf, 0xef7d, 0xffde, 0xffde, 0xf79d, 0xf79d, 
    0xe75d, 0xef9e, 0xffff, 0xffde, 0xef5c, 0xf7be, 0xffff, 0xe77e, 0xdf7d, 0xffff, 0xfffe, 0xf77c, 0xef7e, 0xf7ff, 0xf7df, 0xf75b, 0xf77d, 0xffff, 0xf79e, 0xef5d, 0xffbe, 0xffff, 0xef7d, 0xef5d, 
    0xffff, 0xffdf, 0xf77d, 0xf79e, 0xffff, 0xf79e, 0xef5d, 0xffff, 0xffff, 0xf7bd, 0xf79c, 0xffde, 0xffff, 0xf7be, 0xf79d, 0xffde, 0xffff, 0xef7d, 0xf7be, 0xffff, 0xf7be, 0xf79e, 0xffff, 0xffff, 
    0xffff, 0xffff, 0xf77d, 0xf77e, 0xffff, 0xffdf, 0xef5d, 0xf7df, 0xffff, 0xf79d, 0xe79d, 0xf7ff, 0xffff, 0xef7d, 0xef7d, 0xffff, 0xf7be, 0xef5d, 0xf7be, 0xffff, 0xef9e, 0xe75d, 0xf7bf, 0xffff
};