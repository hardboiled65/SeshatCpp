#include "normalization.h"

#include <seshat/codepoint.h>
#include <seshat/unicode/normalize.h>

#include <array>
#include <iostream>
#include <cstdlib>

using seshat::CodePoint;
using seshat::CodePointSequence;
using S = CodePointSequence;

CodePointSequence test_table_old[][5] = {
// @Part0 # Specific cases
    { S{ 0x1e0a }, S{ 0x1e0a }, S{ 0x44, 0x307 }, S{ 0x1e0a }, S{ 0x44, 0x307 } },
    { S{ 0x1e0c }, S{ 0x1e0c }, S{ 0x44, 0x323 }, S{ 0x1e0c }, S{ 0x44, 0x323 } },
    { S{ 0x1e0a, 0x323 }, S{ 0x1e0c, 0x307 }, S{ 0x44, 0x323, 0x307 }, S{ 0x1e0c, 0x307 }, S{ 0x44, 0x323, 0x307 } },
    { S{ 0x1e0c, 0x307 }, S{ 0x1e0c, 0x307 }, S{ 0x44, 0x323, 0x307 }, S{ 0x1e0c, 0x307 }, S{ 0x44, 0x323, 0x307 } },
    { S{ 0x44, 0x307, 0x323 }, S{ 0x1e0c, 0x307 }, S{ 0x44, 0x323, 0x307 }, S{ 0x1e0c, 0x307 }, S{ 0x44, 0x323, 0x307 } },
    { S{ 0x44, 0x323, 0x307 }, S{ 0x1e0c, 0x307 }, S{ 0x44, 0x323, 0x307 }, S{ 0x1e0c, 0x307 }, S{ 0x44, 0x323, 0x307 } },
    { S{ 0x1e0a, 0x31b }, S{ 0x1e0a, 0x31b }, S{ 0x44, 0x31b, 0x307 }, S{ 0x1e0a, 0x31b }, S{ 0x44, 0x31b, 0x307 } },
    { S{ 0x1e0c, 0x31b }, S{ 0x1e0c, 0x31b }, S{ 0x44, 0x31b, 0x323 }, S{ 0x1e0c, 0x31b }, S{ 0x44, 0x31b, 0x323 } },
    { S{ 0x1e0a, 0x31b, 0x323 }, S{ 0x1e0c, 0x31b, 0x307 }, S{ 0x44, 0x31b, 0x323, 0x307 }, S{ 0x1e0c, 0x31b, 0x307 }, S{ 0x44, 0x31b, 0x323, 0x307 } },
    { S{ 0x1e0c, 0x31b, 0x307 }, S{ 0x1e0c, 0x31b, 0x307 }, S{ 0x44, 0x31b, 0x323, 0x307 }, S{ 0x1e0c, 0x31b, 0x307 }, S{ 0x44, 0x31b, 0x323, 0x307 } },
    { S{ 0x44, 0x31b, 0x307, 0x323 }, S{ 0x1e0c, 0x31b, 0x307 }, S{ 0x44, 0x31b, 0x323, 0x307 }, S{ 0x1e0c, 0x31b, 0x307 }, S{ 0x44, 0x31b, 0x323, 0x307 } },
    { S{ 0x44, 0x31b, 0x323, 0x307 }, S{ 0x1e0c, 0x31b, 0x307 }, S{ 0x44, 0x31b, 0x323, 0x307 }, S{ 0x1e0c, 0x31b, 0x307 }, S{ 0x44, 0x31b, 0x323, 0x307 } },
    { S{ 0xc8 }, S{ 0xc8 }, S{ 0x45, 0x300 }, S{ 0xc8 }, S{ 0x45, 0x300 } },
    { S{ 0x112 }, S{ 0x112 }, S{ 0x45, 0x304 }, S{ 0x112 }, S{ 0x45, 0x304 } },
    { S{ 0x45, 0x300 }, S{ 0xc8 }, S{ 0x45, 0x300 }, S{ 0xc8 }, S{ 0x45, 0x300 } },
    { S{ 0x45, 0x304 }, S{ 0x112 }, S{ 0x45, 0x304 }, S{ 0x112 }, S{ 0x45, 0x304 } },
    { S{ 0x1e14 }, S{ 0x1e14 }, S{ 0x45, 0x304, 0x300 }, S{ 0x1e14 }, S{ 0x45, 0x304, 0x300 } },
    { S{ 0x112, 0x300 }, S{ 0x1e14 }, S{ 0x45, 0x304, 0x300 }, S{ 0x1e14 }, S{ 0x45, 0x304, 0x300 } },
    { S{ 0x1e14, 0x304 }, S{ 0x1e14, 0x304 }, S{ 0x45, 0x304, 0x300, 0x304 }, S{ 0x1e14, 0x304 }, S{ 0x45, 0x304, 0x300, 0x304 } },
    { S{ 0x45, 0x304, 0x300 }, S{ 0x1e14 }, S{ 0x45, 0x304, 0x300 }, S{ 0x1e14 }, S{ 0x45, 0x304, 0x300 } },
    { S{ 0x45, 0x300, 0x304 }, S{ 0xc8, 0x304 }, S{ 0x45, 0x300, 0x304 }, S{ 0xc8, 0x304 }, S{ 0x45, 0x300, 0x304 } },
    { S{ 0x5b8, 0x5b9, 0x5b1, 0x591, 0x5c3, 0x5b0, 0x5ac, 0x59f }, S{ 0x5b1, 0x5b8, 0x5b9, 0x591, 0x5c3, 0x5b0, 0x5ac, 0x59f }, S{ 0x5b1, 0x5b8, 0x5b9, 0x591, 0x5c3, 0x5b0, 0x5ac, 0x59f }, S{ 0x5b1, 0x5b8, 0x5b9, 0x591, 0x5c3, 0x5b0, 0x5ac, 0x59f }, S{ 0x5b1, 0x5b8, 0x5b9, 0x591, 0x5c3, 0x5b0, 0x5ac, 0x59f } },
    { S{ 0x592, 0x5b7, 0x5bc, 0x5a5, 0x5b0, 0x5c0, 0x5c4, 0x5ad }, S{ 0x5b0, 0x5b7, 0x5bc, 0x5a5, 0x592, 0x5c0, 0x5ad, 0x5c4 }, S{ 0x5b0, 0x5b7, 0x5bc, 0x5a5, 0x592, 0x5c0, 0x5ad, 0x5c4 }, S{ 0x5b0, 0x5b7, 0x5bc, 0x5a5, 0x592, 0x5c0, 0x5ad, 0x5c4 }, S{ 0x5b0, 0x5b7, 0x5bc, 0x5a5, 0x592, 0x5c0, 0x5ad, 0x5c4 } },
    { S{ 0x1100, 0xac00, 0x11a8 }, S{ 0x1100, 0xac01 }, S{ 0x1100, 0x1100, 0x1161, 0x11a8 }, S{ 0x1100, 0xac01 }, S{ 0x1100, 0x1100, 0x1161, 0x11a8 } },
    { S{ 0x1100, 0xac00, 0x11a8, 0x11a8 }, S{ 0x1100, 0xac01, 0x11a8 }, S{ 0x1100, 0x1100, 0x1161, 0x11a8, 0x11a8 }, S{ 0x1100, 0xac01, 0x11a8 }, S{ 0x1100, 0x1100, 0x1161, 0x11a8, 0x11a8 } },
    { S{ 0x115b9, 0x334, 0x115af }, S{ 0x115b9, 0x334, 0x115af }, S{ 0x115b9, 0x334, 0x115af }, S{ 0x115b9, 0x334, 0x115af }, S{ 0x115b9, 0x334, 0x115af } }
};

CodePointSequence str_to_seq(const char *str)
{
    CodePointSequence seq;
    char *end;
    for (uint32_t i = std::strtoul(str, &end, 16); str != end; i = std::strtoul(str, &end, 16)) {
        seq.append(CodePoint(i));
        str = end;
    }
    return seq;
}

int main()
{
    // Test reordering
    std::cout << "Test reordering" << std::endl;
    CodePointSequence s = { 0x0044, 0x0307, 0x0323 };
    for (auto cp: s) {
        std::cout << cp.to_string() << " ";
    }
    std::cout << std::endl;
    seshat::unicode::canonical_ordering(s);
    for (auto cp: s) {
        std::cout << cp.to_string() << " ";
    }
    std::cout << std::endl;
    // Test nfd
    std::cout << "Test nfd" << std::endl;
    CodePointSequence org = { 0x0044, 0x0307, 0x0323 };
    for (auto cp: org) {
        std::cout << cp.to_string() << " ";
    }
    std::cout << std::endl;
    auto decomposed = seshat::unicode::nfd(org);
    for (auto cp: decomposed) {
        std::cout << cp.to_string() << " ";
    }
    std::cout << std::endl;
    // Test str_to_seq
    std::cout << "Test str_to_seq()" << std::endl;
    const char *test_str = "0044 0303";
    auto test_seq = str_to_seq(test_str);
    std::cout << "length(2): " << test_seq.length() << std::endl;
    std::cout << "\n----------------------------" << std::endl;

    // Main test
    for (const char **seqs: test_table) {
        CodePointSequence org = str_to_seq(seqs[0]);
        CodePointSequence seshat_nfd = seshat::unicode::nfd(org);
        CodePointSequence right = str_to_seq(seqs[2]);
        for (auto cp: org) {
            std::cout << cp.to_string() << " ";
        }
        if (str_to_seq(seqs[2]) != seshat_nfd) {
            std::cout << ": not => ";
            for (auto cp: seshat_nfd) {
                std::cout << cp.to_string() << " ";
            }
            std::cout << ", but should ";
            for (auto cp: right) {
                std::cout << cp.to_string() << " ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
