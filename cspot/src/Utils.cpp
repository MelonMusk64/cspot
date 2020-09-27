#include "Utils.h"
#include "SHA1.h"
#include "pb_encode.h"

std::vector<uint8_t> blockRead(int fd, size_t readSize)
{
	std::vector<uint8_t> buf(readSize);
	unsigned int idx = 0;
	ssize_t n;

	while (idx < readSize)
	{
		if ((n = recv(fd, &buf[idx], readSize - idx, 0)) <= 0)
		{
			return buf;
		}
		idx += n;
	}
	return buf;
}
unsigned long long getCurrentTimestamp()
{
	unsigned long long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return now;
}

uint64_t hton64(uint64_t value) {
    int num = 42;
    if (*(char *)&num == 42) {
        uint32_t high_part = htonl((uint32_t)(value >> 32));
        uint32_t low_part = htonl((uint32_t)(value & 0xFFFFFFFFLL));
        return (((uint64_t)low_part) << 32) | high_part;
    } else {
        return value;
    }
}

std::string bytesToHexString(std::vector<uint8_t>& v) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    std::vector<uint8_t>::const_iterator it;

    for (it = v.begin(); it != v.end(); it++) {
        ss << std::setw(2) << static_cast<unsigned>(*it);
    }

    return ss.str();
}

ssize_t blockWrite(int fd, std::vector<uint8_t> data)
{
	unsigned int idx = 0;
	ssize_t n;

	while (idx < data.size())
	{
		if ((n = send(fd, &data[idx], data.size() - idx < 64 ? data.size() - idx : 64, 0)) <= 0)
		{
			return data.size();
		}
		idx += n;
	}
	return data.size();
}

std::vector<uint8_t> SHA1HMAC(std::vector<uint8_t> &inputKey, std::vector<uint8_t> &message)
{
	std::vector<uint8_t> digest = inputKey;
	std::vector<uint8_t> ipad(HMAC_SHA1_BLOCKSIZE);
	std::vector<uint8_t> opad(HMAC_SHA1_BLOCKSIZE);
	unsigned int i;
	auto sha1 = std::make_unique<SHA1>();

	/* Shorten key if needed */
	if (inputKey.size() > HMAC_SHA1_BLOCKSIZE)
	{
		sha1->update(inputKey);
		digest = sha1->finalBytes();
	}

	/* Padding */
	memset(&ipad[0], 0x36, HMAC_SHA1_BLOCKSIZE);
	memset(&opad[0], 0x5c, HMAC_SHA1_BLOCKSIZE);

	/* Pad key */
	for (i = 0; i < digest.size(); i++)
	{
		ipad[i] ^= digest[i];
		opad[i] ^= digest[i];
	}

	/* First */
	sha1 = std::make_unique<SHA1>();
	sha1->update(ipad);
	sha1->update(message);
	digest = sha1->finalBytes();

	/* Second */
	sha1 = std::make_unique<SHA1>();
	sha1->update(opad);
	sha1->update(digest);

	return sha1->finalBytes();
}