// get-top-asm-instructrions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>

#include <libpe.h>
#include <distorm.h>

using namespace libpe;
using libpe_ptr = IlibpeShPtr;
typedef std::pair<std::string, int> pair;

// How many instructions to allocate on stack.
#define MAX_INSTRUCTIONS 0x1000

int main()
{
	libpe_ptr pLibpe{ Createlibpe() };
	if (pLibpe->LoadPe(L"hello_na.exe") != S_OK) {
		std::cout << "Failed to Load PE file" << std::endl;
		exit(EXIT_FAILURE);
	}

	DWORD StartCodeSection, SizeCodeSection;
	PLIBPE_SECHEADERS_VEC SectionHeaders;
	pLibpe->GetSectionsHeaders(SectionHeaders);

	for (auto& ItSections : *SectionHeaders)
	{
		std::string currentSectionName(reinterpret_cast<char const*>(ItSections.stSecHdr.Name));
		if ((currentSectionName.compare(".text")) == 0) {
			StartCodeSection = ItSections.stSecHdr.PointerToRawData;
			SizeCodeSection = ItSections.stSecHdr.SizeOfRawData;
			break;
		}
	}

	if (StartCodeSection == 0 || SizeCodeSection == 0) {
		std::cout << "Failed to locate code section" << std::endl;
		exit(EXIT_FAILURE);
	}

	char * CodeSectionData = new char[SizeCodeSection];
	std::ifstream is("hello_na.exe", std::ifstream::binary);
	if (is) {
		is.seekg(StartCodeSection);
		is.read(CodeSectionData, SizeCodeSection);
		if (!is) {
			std::cout << "Failed to read code section from file";
			exit(EXIT_FAILURE);
		}
	}

	// Holds the result of the decoding.
	_DecodeResult res;

	// Default offset for buffer is 0.
	_OffsetType offset = 0;

	// Decoded instruction information - the Decode will write the results here.
	_DecodedInst decodedInstructions[MAX_INSTRUCTIONS];

	// decodedInstructionsCount indicates how many instructions were written to the result array.
	unsigned int decodedInstructionsCount = 0;

	// Default decoding mode is 32 bits.
	_DecodeType dt = Decode32Bits;

	// Decode the buffer at given offset (virtual address).
	res = distorm_decode(offset, (const unsigned char*)CodeSectionData, SizeCodeSection, dt, decodedInstructions, MAX_INSTRUCTIONS, &decodedInstructionsCount);
	if (res == DECRES_INPUTERR) {
		std::cout << "Failed to decode buffer" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::map<std::string, int> m;
	for (unsigned int i = 0; i < decodedInstructionsCount; i++) {
		printf("%0*I64x (%02d) %-24s %s%s%s\n", dt != Decode64Bits ? 8 : 16, decodedInstructions[i].offset, decodedInstructions[i].size, (char*)decodedInstructions[i].instructionHex.p, (char*)decodedInstructions[i].mnemonic.p, decodedInstructions[i].operands.length != 0 ? " " : "", (char*)decodedInstructions[i].operands.p);
		std::string mnemonic(reinterpret_cast<char*>(decodedInstructions[i].mnemonic.p));
		if (m.find(mnemonic) == m.end()) {
			m[mnemonic] = 1;
		}
		else {
			m[mnemonic]++;
		}
	}

	// sort the map.
	std::vector<pair> vec;
	std::copy(m.begin(), m.end(), std::back_inserter<std::vector<pair>>(vec));
	std::sort(vec.begin(), vec.end(),
		[](const pair& l, const pair& r) {
		if (l.second != r.second)
			return l.second < r.second;
		return l.first < r.first;
	});

	// print the vector
	for (auto const &pair : vec)
		std::cout << '{' << pair.first << "," << pair.second << '}' << '\n';

	delete[] CodeSectionData;
	is.close();
}
