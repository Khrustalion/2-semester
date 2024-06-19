#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <fstream>
#include <stdint.h>
#include <bitset>

const int MEM_SIZE = 262144; // 2^ADDR_LEN
const int ADDR_LEN = 18;
const int CACHE_WAY = 4;
const int CACHE_TAG_LEN = 8; // ADDR_LEN - CACHE_INDEX_LEN - CACHE_OFFSET_LEN
const int CACHE_INDEX_LEN = 5;// log_2(CACHE_SETS)
const int CACHE_OFFSET_LEN = 5; // log_2(CACHE_LINE_SIZE)
const int CACHE_SIZE = 4096; // CACHE_LINE_COUNT * CACHE_LINE_SIZE
const int CACHE_LINE_SIZE = 32;
const int CACHE_LINE_COUNT = 128; // CACHE_SETS * CACHE_WAY
const int CACHE_SETS = 32;

std::map<std::string, int> number {
    {"zero", 0},
    {"x0", 0},
    {"x1", 1},
    {"ra", 1},
    {"x2", 2},
    {"sp", 2},
    {"x3", 3},
    {"gp", 3},
    {"x4", 4},
    {"tp", 4},
    {"x5", 5},
    {"t0", 5},
    {"x6", 6},
    {"t1", 6},
    {"x7", 7},
    {"t2", 7},
    {"x8", 8},
    {"s0", 8},
    {"fp", 8},
    {"x9", 9},
    {"s1", 9},
    {"x10", 10},
    {"a0", 10},
    {"x11", 11},
    {"a1", 11},
    {"x12", 12},
    {"a2", 12},
    {"x13", 13},
    {"a3", 13},
    {"x14", 14},
    {"a4", 14},
    {"x15", 15},
    {"a5", 15},
    {"x16", 16},
    {"a6", 16},
    {"x17", 17},
    {"a7", 17},
    {"x18", 18},
    {"s2", 18},
    {"x19", 19},
    {"s3", 19},
    {"x20", 20},
    {"s4", 20},
    {"x21", 21},
    {"s5", 21},
    {"x22", 22},
    {"s6", 22},
    {"x23", 23},
    {"s7", 23},
    {"x24", 24},
    {"s8", 24},
    {"x25", 25},
    {"s9", 25},
    {"x26", 26},
    {"s10", 26},
    {"x27", 27},
    {"s11", 27},
    {"x28", 28},
    {"t3", 28},
    {"x29", 29},
    {"t4", 29},
    {"x30", 30},
    {"t5", 30},
    {"x31", 31},
    {"t6", 31},
};

struct parseArgument {
    int replacement_;
    std::string asm_ = "";
    std::string bin_ = "";
};

int stoi_(std::string s) {
    if (s[1] == 'x') {
        return std::stoi(s, nullptr, 16);
    }
    return std::stoi(s);
}

struct CacheLine {
    uint8_t data[CACHE_LINE_SIZE];
    bool used = false;
    uint16_t tag;
    uint8_t flag = 0;

    CacheLine() = default;

    ~CacheLine() = default;
};

struct CacheBlock {
    CacheLine line[CACHE_WAY];

    CacheBlock() = default;

    ~CacheBlock() = default;
};

struct Processor {
    CacheBlock cache[CACHE_SETS];
    uint8_t ram[MEM_SIZE];
 
    uint64_t cache_hit = 0;
    uint64_t cache_miss = 0;

    Processor() = default;

    void storeByte(uint32_t adr, uint8_t byte, bool calc = true) {
        uint16_t index = static_cast<uint16_t>((adr >> CACHE_OFFSET_LEN) % (1 << CACHE_INDEX_LEN));
        uint16_t tag = static_cast<uint16_t>((adr >> (CACHE_OFFSET_LEN + CACHE_INDEX_LEN)) % (1 << CACHE_TAG_LEN));
        uint16_t offset = static_cast<uint16_t>(adr % (1 << CACHE_OFFSET_LEN));

        int way = this->findInCache(adr);

        if (way == -1) {
            if (calc) {
                ++cache_miss;
            }
            this->insertInCache(adr);
            way = this->findInCache(adr);
        }
        else if (calc) {
            updateFlag(index, cache[index].line[way].flag);
            ++cache_hit;
        }

        cache[index].line[way].data[offset] = byte;
    }

    void storeHalf(uint32_t adr, uint16_t half, bool calc = true) {
        uint16_t index = static_cast<uint16_t>((adr >> CACHE_OFFSET_LEN) % (1 << CACHE_INDEX_LEN));
        uint32_t adr_2 = adr + 1;

        int way_1 = this->findInCache(adr);
        int way_2 = this->findInCache(adr_2);

        if (((way_1 == -1) || (way_2 == -1)) && calc) {
            ++cache_miss;
        }
        else if (calc) {
            updateFlag(index, cache[index].line[way_1].flag);
            ++cache_hit;
        }

        uint8_t byte_1 = static_cast<uint8_t>(half % (1 << 8));
        uint8_t byte_2 = static_cast<uint8_t>(half >> 8);

        this->storeByte(adr, byte_1, false);
        this->storeByte(adr_2, byte_2, false);
    }

    void storeWord(uint32_t adr, uint32_t word) {
        uint16_t index = static_cast<uint16_t>((adr >> CACHE_OFFSET_LEN) % (1 << CACHE_INDEX_LEN));
        uint32_t adr_2 = adr + 2;

        int way_1 = this->findInCache(adr);
        int way_2 = this->findInCache(adr + 1);
        int way_3 = this->findInCache(adr_2);
        int way_4 = this->findInCache(adr_2 + 1);

        if ((way_1 == -1) || (way_2 == -1) || (way_3 == -1) || (way_4 == -1)) {
            ++cache_miss;
        }
        else {
            updateFlag(index, cache[index].line[way_1].flag);
            ++cache_hit;
        }

        uint16_t half_1 = static_cast<uint16_t>(word % (1 << 16));
        uint16_t half_2 = static_cast<uint16_t>(word >> 16);

        this->storeHalf(adr, half_1, false);
        this->storeHalf(adr_2, half_2, false);
    }

    uint8_t loadByte(uint32_t adr, bool calc = true) {
        uint16_t index = static_cast<uint16_t>((adr >> CACHE_OFFSET_LEN) % (1 << CACHE_INDEX_LEN));
        uint16_t tag = static_cast<uint16_t>((adr >> (CACHE_OFFSET_LEN + CACHE_INDEX_LEN)) % (1 << CACHE_TAG_LEN));
        uint16_t offset = static_cast<uint16_t>(adr % (1 << CACHE_OFFSET_LEN));

        int way = this->findInCache(adr);

        if (way != -1) {
            if (calc) {
                updateFlag(index, cache[index].line[way].flag);
                ++cache_hit;
            }
            return cache[index].line[way].data[offset];
        }
        if (calc) {
            ++cache_miss;
        }
        this->insertInCache(adr);

        return this->loadByte(adr, false);        
    }

    uint16_t loadHalf(uint32_t adr, bool calc = true) {
        uint16_t index = static_cast<uint16_t>((adr >> CACHE_OFFSET_LEN) % (1 << CACHE_INDEX_LEN));
        uint32_t adr_2 = adr + 1;

        int way_1 = this->findInCache(adr);
        int way_2 = this->findInCache(adr_2);

        if (((way_1 == -1) || (way_2 == -1)) && calc) {
            ++cache_miss;
        }
        else if (calc) {
            updateFlag(index, cache[index].line[way_1].flag);
            ++cache_hit;
        }

        uint8_t byte_1 = this->loadByte(adr, false);
        uint8_t byte_2 = this->loadByte(adr_2, false);

        return (static_cast<uint16_t>(byte_2) << 8) | byte_1;
    }

    uint32_t loadWord(uint32_t adr) {
        uint16_t index = static_cast<uint16_t>((adr >> CACHE_OFFSET_LEN) % (1 << CACHE_INDEX_LEN));
        uint32_t adr_2 = adr + 2;

        int way_1 = this->findInCache(adr);
        int way_2 = this->findInCache(adr + 1);
        int way_3 = this->findInCache(adr_2);
        int way_4 = this->findInCache(adr_2 + 1);

        if ((way_1 == -1) || (way_2 == -1) || (way_3 == -1) || (way_4 == -1)) {
            ++cache_miss;
        }
        else {
            updateFlag(index, cache[index].line[way_1].flag);
            ++cache_hit;
        }

        uint16_t half_1 = this->loadHalf(adr, false);
        uint16_t half_2 = this->loadHalf(adr_2, false);

        return (static_cast<uint32_t>(half_2) << 16) | half_1;
    }

    int findInCache(uint32_t adr) {
        uint16_t index = static_cast<uint16_t>((adr >> CACHE_OFFSET_LEN) % (1 << CACHE_INDEX_LEN));
        uint16_t tag = static_cast<uint16_t>((adr >> (CACHE_OFFSET_LEN + CACHE_INDEX_LEN)) % (1 << CACHE_TAG_LEN));

        for (int i = 0; i < CACHE_WAY; ++i) {
            if (cache[index].line[i].used && (tag == cache[index].line[i].tag)) {
                return i;
            }
        }
        return -1;
    }

    void updateFlag(uint16_t index, uint8_t flag) {
        for (int i = 0; i < CACHE_WAY; ++i) {
            if (cache[index].line[i].used && cache[index].line[i].flag < flag) {
                cache[index].line[i].flag = (cache[index].line[i].flag + 1) % CACHE_WAY;
            }
            else if (cache[index].line[i].used && cache[index].line[i].flag == flag) {
                cache[index].line[i].flag = 0;
            }
        }
    }

    void insertInCache(uint32_t adr) {
        uint16_t index = static_cast<uint16_t>((adr >> CACHE_OFFSET_LEN) % (1 << CACHE_INDEX_LEN));
        uint16_t tag = static_cast<uint16_t>((adr >> (CACHE_OFFSET_LEN + CACHE_INDEX_LEN)) % (1 << CACHE_TAG_LEN));

        uint8_t lru = 0;

        for (int i = 0; i < CACHE_WAY; ++i) {
            if (!cache[index].line[i].used) {
                lru = i;
                break;
            }
            if (cache[index].line[lru].flag < cache[index].line[i].flag) {
                lru = i;
            }
        }

        if (cache[index].line[lru].used) {
            int old_adr = (((cache[index].line[lru].tag << CACHE_INDEX_LEN) | index) << CACHE_OFFSET_LEN);

            for (int i = 0; i < CACHE_LINE_SIZE; ++i) {
                ram[old_adr + i] = cache[index].line[lru].data[i];
            }
        }
        
        cache[index].line[lru].flag = 3;
        cache[index].line[lru].tag = tag;
        cache[index].line[lru].used = true;

        adr = ((adr >> CACHE_OFFSET_LEN) << CACHE_OFFSET_LEN);
        
        for (int i = 0; i < CACHE_LINE_SIZE; ++i) {
            cache[index].line[lru].data[i] = ram[adr + i];
        }

        updateFlag(index,  3);
    }   

    void print() {
        for (int i = 0; i < 32; ++i) {
            std::cout << "cache block " << i << "\n";
            for (int j = 0; j < 4; ++j) {
                std::cout << "\tcache line " << j << '\n';
                std::cout << "\t\t";
                std::cout << (int)cache[i].line[j].flag << " " << (int)cache[i].line[j].used << " " << (int)cache[i].line[j].tag << '\n';
            }
        }

        std::cout << "ram: \n\t";

        for (int i = 0; i < 64; ++i) {
            std::cout << (int)ram[i] << " ";
        }
        std::cout << '\n';
    }
};

void parse(int argc, char** argv, parseArgument& argument) {
    for (int i = 1; i < argc; ++i) {
        if (!std::strcmp(argv[i], "--replacement")) {
            argument.replacement_ = stoi_(argv[++i]);
        }
        else if (!std::strcmp(argv[i], "--asm")) {
            argument.asm_ = static_cast<std::string>(argv[++i]);
        }
        else if (!std::strcmp(argv[i], "--bin")) {
            argument.bin_ = static_cast<std::string>(argv[++i]);
        }
    }
}

struct Comands {
    std::vector<std::vector<std::string>> comands_;

    void push(std::string line) {
        std::vector<std::string> comand;
        std::string word = "";

        for (auto c : line) {
            if (c == '/') {
                break;
            }
            else if (c == ' ' || c == '\t') {
                if (word.size()) comand.push_back(word);
                word = "";
                continue;
            }
            else if (c == ',') {
                continue;
            }
            word += c;
        }
        if (word.size()) comand.push_back(word);

        comands_.push_back(comand);
    }   
};

struct Simulate {
    Processor cpu;
    Comands comands;
    uint32_t registers[32];
    uint64_t pc = 0;
    std::ofstream file_out;
    int replacement = -1;
    bool write = false;

    Simulate(std::string in, int new_replacement, std::string out = "") {
        replacement = new_replacement;
        registers[0] = 0;
        std::ifstream file_input(in);
        std::string line;
        while (std::getline(file_input, line)) {
            comands.push(line);
        }
        file_input.close();
        if (out.size() != 0) {
            file_out = std::ofstream(out);
            write = true;
        }
    }

    void writeCode(uint32_t code) {
        if (write) {
            for (int i = 0; i < 4; ++i) {
                file_out << static_cast<char>((code >> (8*i)) % (1 << 8));
            }
        }
    }

    uint32_t getCode1Reg(std::string start_code, uint32_t imm, uint8_t rd) {
        uint32_t code = std::stoi(start_code, nullptr, 2);
        code |= (imm << 12);
        code |= (rd << 7);

        return code;
    }

    uint32_t getCode1RegJal(std::string start_code, uint32_t imm, uint8_t rd) {
        uint32_t code = std::stoi(start_code, nullptr, 2);
        code |= (((imm >> 12) % (1 << 8)) << 12);
        code |= (((imm >> 11) & 1) << 20);
        code |= (((imm >> 1) % (1 << 10)) << 21);
        code |= (((imm >> 20) & 1) << 31);
        code |= (rd << 7);

        return code;
    }

    uint32_t getCode2Reg(std::string start_code, uint16_t imm, uint8_t rd, uint8_t rs1) { 
        uint32_t code = std::stoi(start_code, nullptr, 2);
        code |= (imm << 20);
        code |= (rs1 << 15);
        code |= (rd << 7);

        return code;
    }

    uint32_t getCode2RegStore(std::string start_code, uint16_t offset, uint8_t rs1, uint8_t rs2) {
        uint32_t code = std::stoi(start_code, nullptr, 2);
        code |= ((offset % (1 << 5)) << 7);
        code |= ((offset >> 5) << 25);
        code |= (rs2 << 20);
        code |= (rs1 << 15);

        return code;
    }

    uint32_t getCode2RegB(std::string start_code, uint16_t offset, uint8_t rs1, uint8_t rs2) {
        uint32_t code = std::stoi(start_code, nullptr, 2);
        code |= (((offset >> 11) & 1) << 7);
        code |= (((offset >> 1) % (1 << 4)) << 8);
        code |= (((offset >> 5) % (1 << 6)) << 25);
        code |= (((offset >> 12) & 1) << 31);
        code |= (rs1 << 15);
        code |= (rs2 << 20);

        return code;
    }

    uint32_t getCode3Reg(std::string start_code, uint8_t rd, uint8_t rs1, uint8_t rs2) {
        uint32_t code = std::stoi(start_code, nullptr, 2);
        code |= (rs2 << 20);
        code |= (rs1 << 15);
        code |= (rd << 7);

        return code;
    }

    void mul(uint8_t rd, uint8_t rs1, uint8_t rs2) {
        uint32_t x_rs1 = registers[rs1];
        uint32_t x_rs2 = registers[rs2];

        registers[rd] = static_cast<uint32_t>(x_rs1 * x_rs2);
    }

    void mulh(uint8_t rd, uint8_t rs1, uint8_t rs2) {
        uint64_t x_rs1 = static_cast<uint64_t>(registers[rs1]);
        uint64_t x_rs2 = static_cast<uint64_t>(registers[rs2]);

        registers[rd] = static_cast<uint32_t>((x_rs1 * x_rs2) >> 32);
    }

    void mulhsu(uint8_t rd, uint8_t rs1, uint8_t rs2) {
        uint64_t x_rs1 = static_cast<uint64_t>(registers[rs1]);
        uint64_t x_rs2 = static_cast<uint64_t>(registers[rs2]);

        registers[rd] = static_cast<uint32_t>((x_rs1 * x_rs2) >> 32);
    }

    void mulhu(uint8_t rd, uint8_t rs1, uint8_t rs2) {
        uint64_t x_rs1 = static_cast<uint64_t>(registers[rs1]);
        uint64_t x_rs2 = static_cast<uint64_t>(registers[rs2]);

        registers[rd] = static_cast<uint32_t>((x_rs1 * x_rs2) >> 32);
    }

    void div(uint8_t rd, uint8_t rs1, uint8_t rs2) {
        int32_t x_rs1 = static_cast<int32_t>(registers[rs1]);
        int32_t x_rs2 = static_cast<int32_t>(registers[rs2]);

        registers[rd] = static_cast<uint32_t>(x_rs1 / x_rs2);
    }

    void divu(uint8_t rd, uint8_t rs1, uint8_t rs2) {
        uint32_t x_rs1 = registers[rs1];
        uint32_t x_rs2 = registers[rs2];

        registers[rd] = static_cast<uint32_t>(x_rs1 / x_rs2);
    }

    void rem(uint8_t rd, uint8_t rs1, uint8_t rs2) {
        int32_t x_rs1 = static_cast<int32_t>(registers[rs1]);
        int32_t x_rs2 = static_cast<int32_t>(registers[rs2]);

        registers[rd] = static_cast<uint32_t>(x_rs1 % x_rs2);
    }

    void remu(uint8_t rd, uint8_t rs1, uint8_t rs2) {
        uint32_t x_rs1 = registers[rs1];
        uint32_t x_rs2 = registers[rs2];

        registers[rd] = static_cast<uint32_t>(x_rs1 % x_rs2);
    }

    void lui(uint8_t rd, uint32_t imm) {
        imm = (imm >> 12);
        registers[rd] = (imm << 12);
    }

    void auipc(uint8_t rd, int32_t imm) {
        imm = (imm >> 12);
        registers[rd] =  pc + (imm << 12);
    }

    void addi(uint8_t rd, uint8_t rs1, int32_t imm) {
        imm = imm % (1 << 12);
        uint32_t x_rs1 = registers[rs1];

        registers[rd] = static_cast<uint32_t>(x_rs1 + imm);
    }

    void slti(uint8_t rd, uint8_t rs1, int32_t imm) {
        imm = imm % (1 << 12);
        int32_t x_rs1 = static_cast<int32_t>(registers[rs1]);

        registers[rd] = static_cast<uint32_t>(x_rs1 < imm);
    }

    void sltiu(uint8_t rd, uint8_t rs1, uint32_t imm) {
        imm = imm % (1 << 12);
        uint32_t x_rs1 = registers[rs1];

        registers[rd] = static_cast<uint32_t>(x_rs1 < imm);
    }

    void xori(uint8_t rd, uint8_t rs1, uint16_t imm) {
        imm = imm % (1 << 12);
        uint32_t x_rs1 = registers[rs1];

        registers[rd] = static_cast<uint32_t>(x_rs1 ^ imm);
    }

    void ori(uint8_t rd, uint8_t rs1, uint32_t imm) {
        imm = imm % (1 << 12);
        uint32_t x_rs1 = registers[rs1];

        registers[rd] = static_cast<uint32_t>(x_rs1 | imm);
    }

    void andi(uint8_t rd, uint8_t rs1, uint32_t imm) {
        imm = imm % (1 << 12);
        uint32_t x_rs1 = registers[rs1];

        registers[rd] = static_cast<uint32_t>(x_rs1 & imm);
    }

    void slli(uint8_t rd, uint8_t rs1, uint8_t shamt) {
        shamt = shamt % (1 << 5);
        uint32_t x_rs1 = registers[rs1];

        registers[rd] = static_cast<uint32_t>(x_rs1 << shamt);
    }

    void srli(uint8_t rd, uint8_t rs1, uint8_t shamt) {
        shamt = shamt % (1 << 5);
        uint32_t x_rs1 = registers[rs1];

        registers[rd] = static_cast<uint32_t>(x_rs1 >> shamt);
    }

    void srai(uint8_t rd, uint8_t rs1, int8_t shamt) {
        shamt = shamt % (1 << 5);
        uint32_t x_rs1 = registers[rs1];

        registers[rd] = static_cast<uint32_t>(x_rs1 >> shamt);
    }

    void add(uint8_t rd, uint8_t rs1, int8_t rs2) {
        uint32_t x_rs1 = registers[rs1];
        uint32_t x_rs2 = registers[rs2];

        registers[rd] = static_cast<uint32_t>(x_rs1 + x_rs2);
    }

    void sub(uint8_t rd, uint8_t rs1, int8_t rs2) {
        uint32_t x_rs1 = registers[rs1];
        uint32_t x_rs2 = registers[rs2];

        registers[rd] = static_cast<uint32_t>(x_rs1 - x_rs2);
    }

    void sll(uint8_t rd, uint8_t rs1, int8_t rs2) {
        uint32_t x_rs1 = registers[rs1];
        uint32_t x_rs2 = registers[rs2] % (1 << 5);

        registers[rd] = static_cast<uint32_t>(x_rs1 << x_rs2);
    }

    void slt(uint8_t rd, uint8_t rs1, int8_t rs2) {
        uint32_t x_rs1 = static_cast<int32_t>(registers[rs1]);
        uint32_t x_rs2 = static_cast<int32_t>(registers[rs2]);

        registers[rd] = static_cast<uint32_t>(x_rs1 < x_rs2);
    }

    void sltu(uint8_t rd, uint8_t rs1, int8_t rs2) {
        uint32_t x_rs1 = registers[rs1];
        uint32_t x_rs2 = registers[rs2];

        registers[rd] = static_cast<uint32_t>(x_rs1 < x_rs2);
    }

    void xor_(uint8_t rd, uint8_t rs1, int8_t rs2) {
        uint32_t x_rs1 = registers[rs1];
        uint32_t x_rs2 = registers[rs2];

        registers[rd] = static_cast<uint32_t>(x_rs1 ^ x_rs2);
    }

    void srl(uint8_t rd, uint8_t rs1, int8_t rs2) {
        int32_t x_rs1 = static_cast<int32_t>(registers[rs1]);
        uint32_t x_rs2 = registers[rs2] % (1 << 5);

        registers[rd] = static_cast<uint32_t>(x_rs1 >> x_rs2);
    }

    void sra(uint8_t rd, uint8_t rs1, int8_t rs2) {
        int32_t x_rs1 = static_cast<int32_t>(registers[rs1]);
        uint32_t x_rs2 = static_cast<int32_t>(registers[rs2]) % (1 << 5);

        registers[rd] = static_cast<uint32_t>(x_rs1 >> x_rs2);
    }

    void or_(uint8_t rd, uint8_t rs1, int8_t rs2) {
        uint32_t x_rs1 = registers[rs1];
        uint32_t x_rs2 = registers[rs2];

        registers[rd] = static_cast<uint32_t>(x_rs1 | x_rs2);
    }

    void and_(uint8_t rd, uint8_t rs1, int8_t rs2) {
        uint32_t x_rs1 = registers[rs1];
        uint32_t x_rs2 = registers[rs2];

        registers[rd] = static_cast<uint32_t>(x_rs1 & x_rs2);
    }

    void lb(uint8_t rd, uint8_t rs1, uint16_t offset) {
        offset = offset % (1 << 12);
        int8_t byte = static_cast<int8_t>(cpu.loadByte(registers[rs1] + offset));

        registers[rd] = static_cast<uint32_t>(byte);
    }

    void lh(uint8_t rd, uint8_t rs1, uint16_t offset) {
        offset = offset % (1 << 12);
        int16_t half = static_cast<int16_t>(cpu.loadHalf(registers[rs1] + offset));

        registers[rd] = static_cast<uint32_t>(half);
    }

    void lw(uint8_t rd, uint8_t rs1, uint16_t offset) {
        offset = offset % (1 << 12);
        int32_t word = static_cast<int32_t>(cpu.loadWord(registers[rs1] + offset));

        registers[rd] = static_cast<uint32_t>(word);
    }

    void lbu(uint8_t rd, uint8_t rs1, uint16_t offset) {
        offset = offset % (1 << 12);
        uint8_t byte = cpu.loadByte(registers[rs1] + offset);

        registers[rd] = static_cast<uint32_t>(byte);
    }

    void lhu(uint8_t rd, uint8_t rs1, uint16_t offset) {
        offset = offset % (1 << 12);
        uint16_t half = cpu.loadHalf(registers[rs1] + offset);

        registers[rd] = static_cast<uint32_t>(half);
    }

    void sb(uint8_t rs1, uint8_t rs2, int16_t offset) {
        offset = offset % (1 << 12);
        uint8_t byte = registers[rs2] % (1 << 8);

        cpu.storeByte(registers[rs1] + offset, byte);
    }

    void sh(uint8_t rs1, uint8_t rs2, uint16_t offset) {
        offset = offset % (1 << 12);
        uint16_t half = registers[rs2] % (1 << 16);

        cpu.storeHalf(registers[rs1]  + offset, half);
    }

    void sw(uint8_t rs1, uint8_t rs2, uint16_t offset) {
        offset = offset % (1 << 12);
        uint32_t word = registers[rs2];

        cpu.storeWord(registers[rs1] + offset, word);
    }

    void jal(uint8_t rd, int32_t offset) {
        offset = offset % (1 << 20);

        registers[rd] += (pc + 4);
        pc += offset;
    }

    void jalr(uint8_t rd, uint8_t rs1, int16_t offset) {
        offset = offset % (1 << 12);
        uint32_t t = pc + 4;
        
        pc = (registers[rs1] + offset)&(~1);
        registers[rd] = t;
    }

    void beq(uint8_t rs1, uint8_t rs2, int16_t offset) {
        offset = offset % (1 << 12);

        if (registers[rs1] == registers[rs2]) {
            pc += offset / 4;
        }
        else {
            ++pc;
        }
    }

    void bne(uint8_t rs1, uint8_t rs2, int16_t offset) {
        offset = offset % (1 << 12);

        if (registers[rs1] != registers[rs2]) {
            pc += offset / 4;
        }
        else {
            ++pc;
        }
    }

    void blt(uint8_t rs1, uint8_t rs2, int16_t offset) {
        offset = offset % (1 << 12);

        if (registers[rs1] < registers[rs2]) {
            pc += offset / 4;
        }
        else {
            ++pc;
        }
    }

    void bge(uint8_t rs1, uint8_t rs2, int16_t offset) {
        offset = offset % (1 << 12);

        if (registers[rs1] >= registers[rs2]) {
            pc += offset / 4;
        }
        else {
            ++pc;
        }
    }

    void bltu(uint8_t rs1, uint8_t rs2, int16_t offset) {
        offset = offset % (1 << 12);

        if (static_cast<int32_t>(registers[rs1]) < static_cast<int32_t>(registers[rs2])) {
            pc += offset / 4;
        }
        else {
            ++pc;
        }
    }

    void bgeu(uint8_t rs1, uint8_t rs2, int16_t offset) {
        offset = offset % (1 << 12);

        if (static_cast<int32_t>(registers[rs1]) >= static_cast<int32_t>(registers[rs2])) {
            pc += offset / 4;
        }
        else {
            ++pc;
        }
    }

    void getByteCode() {
        for (int i = 0; i < comands.comands_.size(); ++i) {
            std::vector<std::string> comand = comands.comands_[i];
            if (comand[0] == "mul") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("10000000000000000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "mulh") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("10000000000001000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "mulhsu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("10000000000010000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "mulhu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("10000000000011000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "div") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("10000000000100000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "divu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("10000000000101000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "rem") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("10000000000110000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "remu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("10000000000111000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "lui") {
                uint8_t rd = number[comand[1]];
                uint32_t imm = static_cast<uint32_t>(stoi_(comand[2]));
                imm = ((imm >> 12) << 12);

                uint32_t code = getCode1Reg("110111", imm, rd);

                writeCode(code);
            }
            else if (comand[0] == "auipc") {
                uint8_t rd = number[comand[1]];
                uint32_t imm = static_cast<uint32_t>(stoi_(comand[2]));

                uint32_t code = getCode1Reg("10111", imm, rd);

                writeCode(code);
            }
            else if (comand[0] == "addi") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int32_t imm = stoi_(comand[3]);

                uint32_t code = getCode2Reg("10011", imm, rd, rs1);
    
                writeCode(code);
            }
            else if (comand[0] == "slti") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int16_t imm = static_cast<int16_t>(stoi_(comand[3]));

                uint32_t code = getCode2Reg("10000000010011", imm, rd, rs1);
    
                writeCode(code);
            }
            else if (comand[0] == "sltiu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint16_t imm = static_cast<uint16_t>(stoi_(comand[3]));

                uint32_t code = getCode2Reg("11000000010011", imm, rd, rs1);
    
                writeCode(code);
            }
            else if (comand[0] == "xori") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint16_t imm = static_cast<uint16_t>(stoi_(comand[3]));

                uint32_t code = getCode2Reg("100000000010011", imm, rd, rs1);
    
                writeCode(code);
            }
            else if (comand[0] == "ori") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint16_t imm = static_cast<uint16_t>(stoi_(comand[3]));

                uint32_t code = getCode2Reg("110000000010011", imm, rd, rs1);
    
                writeCode(code);
            }
            else if (comand[0] == "andi") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint16_t imm = static_cast<uint16_t>(stoi_(comand[3]));

                uint32_t code = getCode2Reg("111000000010011", imm, rd, rs1);
    
                writeCode(code);
            }
            else if (comand[0] == "slli") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t imm = static_cast<uint8_t>(stoi_(comand[3]));

                uint32_t code = getCode2Reg("1000000010011", imm, rd, rs1);
    
                writeCode(code);
            }
            else if (comand[0] == "srli") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t imm = static_cast<uint8_t>(stoi_(comand[3]));

                uint32_t code = getCode2Reg("101000000010011", imm, rd, rs1);
    
                writeCode(code);
            }
            else if (comand[0] == "srai") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int8_t imm = static_cast<int8_t>(stoi_(comand[3]));

                uint32_t code = getCode2Reg("1000000000000000101000000010011", imm, rd, rs1);
    
                writeCode(code);
            }
            else if (comand[0] == "add") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "sub") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("1000000000000000000000000110011", rd, rs1, rs2);
    
                writeCode(code);
            }
            else if (comand[0] == "sll") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("1000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "slt") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("10000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "sltu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("11000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "xor") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("100000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "srl") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("101000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "sra") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("1000000000000000101000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "or") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("110000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "and") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                uint32_t code = getCode3Reg("111000000110011", rd, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "lb") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[3]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                uint32_t code = getCode2Reg("11", offset, rd, rs1);

                writeCode(code);
            }
            else if (comand[0] == "lh") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[3]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                uint32_t code = getCode2Reg("1000000000011", offset, rd, rs1);

                writeCode(code);
            }
            else if (comand[0] == "lw") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[3]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                uint32_t code = getCode2Reg("10000000000011", offset, rd, rs1);

                writeCode(code);
            }   
            else if (comand[0] == "lbu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[3]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                uint32_t code = getCode2Reg("100000000000011", offset, rd, rs1);

                writeCode(code);
            }
            else if (comand[0] == "lhu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[3]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                uint32_t code = getCode2Reg("101000000000011", offset, rd, rs1);

                writeCode(code);
            }
            else if (comand[0] == "sb") {
                uint8_t rs1 = number[comand[3]];
                uint8_t rs2 = number[comand[1]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                uint32_t code = getCode2RegStore("100011", offset, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "sh") {
                uint8_t rs1 = number[comand[3]];
                uint8_t rs2 = number[comand[1]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                uint32_t code = getCode2RegStore("1000000100011", offset, rs1, rs2);

                writeCode(code);
            }
            else if (comand[0] == "sw") {
                uint8_t rs1 = number[comand[3]];
                uint8_t rs2 = number[comand[1]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));


                uint32_t code = getCode2RegStore("10000000100011", offset, rs1, rs2);

                writeCode(code);
            } 
            else if (comand[0] == "beq") {
                uint8_t rs1 = number[comand[1]];
                uint8_t rs2 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                uint32_t code = getCode2RegB("1100011", offset, rs1, rs2);

                writeCode(code);
            } 
            else if (comand[0] == "bne") {
                uint8_t rs1 = number[comand[1]];
                uint8_t rs2 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                uint32_t code = getCode2RegB("1000001100011", offset, rs1, rs2);

                writeCode(code);
            } 
            else if (comand[0] == "blt") {
                uint8_t rs1 = number[comand[1]];
                uint8_t rs2 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                uint32_t code = getCode2RegB("100000001100011", offset, rs1, rs2);

                writeCode(code);
            } 
            else if (comand[0] == "bge") {
                uint8_t rs1 = number[comand[1]];
                uint8_t rs2 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                uint32_t code = getCode2RegB("101000001100011", offset, rs1, rs2);

                writeCode(code);
            } 
            else if (comand[0] == "bltu") {
                uint8_t rs1 = number[comand[1]];
                uint8_t rs2 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                uint32_t code = getCode2RegB("110000001100011", offset, rs1, rs2);

                writeCode(code);
            } 
            else if (comand[0] == "bgeu") {
                uint8_t rs1 = number[comand[1]];
                uint8_t rs2 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                uint32_t code = getCode2RegB("111000001100011", offset, rs1, rs2);

                writeCode(code);
            } 
            else if (comand[0] == "jal") {
                uint8_t rd = number[comand[1]];
                int32_t offset = static_cast<int32_t>(stoi_(comand[2]));

                uint32_t code = getCode1RegJal("1101111", offset, rd);

                writeCode(code);
            }
            else if (comand[0] == "jalr") {
                uint8_t rs1 = number[comand[1]];
                uint8_t rs2 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));
                uint32_t code = getCode2Reg("1100111", offset, rs1, rs2);

                writeCode(code);
            } 
        }

        file_out.close();
    }

    void simulate() {
        while (pc < comands.comands_.size()) {
            std::vector<std::string> comand = comands.comands_[pc];

            if (comand[0] == "mul") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->mul(rd, rs1, rs2);
            }
            else if (comand[0] == "mulh") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->mulh(rd, rs1, rs2);
            }
            else if (comand[0] == "mulhsu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->mulhsu(rd, rs1, rs2);
            }
            else if (comand[0] == "mulhu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->mulhu(rd, rs1, rs2);
            }
            else if (comand[0] == "div") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->div(rd, rs1, rs2);
            }
            else if (comand[0] == "divu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->divu(rd, rs1, rs2);
            }
            else if (comand[0] == "rem") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->rem(rd, rs1, rs2);
            }
            else if (comand[0] == "remu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->remu(rd, rs1, rs2);
            }
            else if (comand[0] == "lui") {
                uint8_t rd = number[comand[1]];
                uint32_t imm = static_cast<uint32_t>(stoi_(comand[2]));

                this->lui(rd, imm);
            }
            else if (comand[0] == "auipc") {
                uint8_t rd = number[comand[1]];
                uint32_t imm = static_cast<uint32_t>(stoi_(comand[2]));

                this->auipc(rd, imm);
            }
            else if (comand[0] == "addi") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int32_t imm = stoi_(comand[3]);

                this->addi(rd, rs1, imm);
            }
            else if (comand[0] == "slti") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int16_t imm = static_cast<int16_t>(stoi_(comand[3]));

                this->slti(rd, rs1, imm);
            }
            else if (comand[0] == "sltiu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint16_t imm = static_cast<uint16_t>(stoi_(comand[3]));

                this->sltiu(rd, rs1, imm);
            }
            else if (comand[0] == "xori") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint16_t imm = static_cast<uint16_t>(stoi_(comand[3]));

                this->xori(rd, rs1, imm);
            }
            else if (comand[0] == "ori") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint16_t imm = static_cast<uint16_t>(stoi_(comand[3]));

                this->ori(rd, rs1, imm);
            }
            else if (comand[0] == "andi") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint16_t imm = static_cast<uint16_t>(stoi_(comand[3]));

                this->andi(rd, rs1, imm);
            }
            else if (comand[0] == "slli") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t imm = static_cast<uint8_t>(stoi_(comand[3]));

                this->slli(rd, rs1, imm);
            }
            else if (comand[0] == "srli") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t imm = static_cast<uint8_t>(stoi_(comand[3]));

                this->srli(rd, rs1, imm);
            }
            else if (comand[0] == "srai") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int8_t imm = static_cast<int8_t>(stoi_(comand[3]));

                this->srai(rd, rs1, imm);
            }
            else if (comand[0] == "add") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->add(rd, rs1, rs2);
            }
            else if (comand[0] == "sub") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->sub(rd, rs1, rs2);
            }
            else if (comand[0] == "sll") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->sll(rd, rs1, rs2);
            }
            else if (comand[0] == "slt") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->slt(rd, rs1, rs2);
            }
            else if (comand[0] == "sltu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->sltu(rd, rs1, rs2);
            }
            else if (comand[0] == "xor") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->xor_(rd, rs1, rs2);
            }
            else if (comand[0] == "srl") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->srl(rd, rs1, rs2);
            }
            else if (comand[0] == "sra") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->sra(rd, rs1, rs2);
            }
            else if (comand[0] == "or") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->or_(rd, rs1, rs2);
            }
            else if (comand[0] == "and") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                uint8_t rs2 = number[comand[3]];

                this->and_(rd, rs1, rs2);
            }
            else if (comand[0] == "lb") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[3]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                this->lb(rd, rs1, offset);
            }
            else if (comand[0] == "lh") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[3]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                this->lh(rd, rs1, offset);
            }
            else if (comand[0] == "lw") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[3]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                this->lw(rd, rs1, offset);
            }   
            else if (comand[0] == "lbu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[3]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                this->lbu(rd, rs1, offset);
            }
            else if (comand[0] == "lhu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[3]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                this->lhu(rd, rs1, offset);
            }
            else if (comand[0] == "sb") {
                uint8_t rs1 = number[comand[3]];
                uint8_t rs2 = number[comand[1]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                this->sb(rs1, rs2, offset);
            }
            else if (comand[0] == "sh") {
                uint8_t rd = number[comand[3]];
                uint8_t rs1 = number[comand[1]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                this->sh(rd, rs1, offset);
            }
            else if (comand[0] == "sw") {
                uint8_t rd = number[comand[3]];
                uint8_t rs1 = number[comand[1]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[2]));

                this->sw(rd, rs1, offset);
            } 
            else if (comand[0] == "beq") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                this->beq(rd, rs1, offset);
                continue;
            } 
            else if (comand[0] == "bne") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                this->bne(rd, rs1, offset);
                continue;

            } 
            else if (comand[0] == "blt") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                this->blt(rd, rs1, offset);
                continue;
            } 
            else if (comand[0] == "bge") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                this->bge(rd, rs1, offset);
                continue;
            } 
            else if (comand[0] == "bltu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                this->bltu(rd, rs1, offset);
                continue;
            } 
            else if (comand[0] == "bgeu") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                this->bgeu(rd, rs1, offset);
                continue;
            } 
            else if (comand[0] == "jal") {
                uint8_t rd = number[comand[1]];
                int32_t offset = static_cast<int32_t>(stoi_(comand[2]));

                this->jal(rd, offset);
                continue;
            }
            else if (comand[0] == "jalr") {
                uint8_t rd = number[comand[1]];
                uint8_t rs1 = number[comand[2]];
                int16_t offset = static_cast<int16_t>(stoi_(comand[3]));

                this->jalr(rd, rs1, offset);
                continue;
            } 
            ++pc;
        }

        if (cpu.cache_hit + cpu.cache_miss == 0 && (replacement == 0 || replacement == 1)) {
        printf("LRU\thit rate: nan\n");
        }
        else if (replacement == 0 || replacement == 1) {
            printf("LRU\thit rate: %3.4f%%\n", static_cast<double>(cpu.cache_hit) / (cpu.cache_hit + cpu.cache_miss) * 100);
        }
        if (replacement == 0 || replacement == 2) {
            printf("pLRU\tunsupported\n");
        }
    }
};

int main(int argc, char** argv) {
    parseArgument argument;
    Comands comands;
    parse(argc, argv, argument);

    Simulate s(argument.asm_, argument.replacement_, argument.bin_);

    if (argument.bin_.size() != 0) {
        s.getByteCode();
    }

    s.simulate();  

    return 0;
}