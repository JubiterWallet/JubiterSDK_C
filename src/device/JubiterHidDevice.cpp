
#include <JUB_SDK.h>

#ifdef HID_MODE

#ifdef _WIN32  // current test windows, may be works in mac/linux ...

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <device/JubiterHidDevice.hpp>
#include <utility/util.hpp>

#include <iostream>  
#include <utility/uchar_vector.h>
namespace jub {
JubiterHidDevice::JubiterHidDevice()
    : pid(PID), vid(VID), m_handle(NULL), firstCmd(true) {
	hid_init();
}

JubiterHidDevice::~JubiterHidDevice() { hid_exit(); }

std::vector<std::string> JubiterHidDevice::enumDevice()
{
	std::vector<std::string> token_list;

	auto hid_dev = hid_enumerate(VID, PID);
	auto hid_dev_head = hid_dev;
	while (hid_dev)
	{
		token_list.push_back(hid_dev->path);
		hid_dev = hid_dev->next;
	}

	hid_free_enumeration(hid_dev_head);
	return token_list;
}

JUB_RV JubiterHidDevice::connect(const std::string path) {
	int ret = hid_init();
	if (ret != 0) {
		return JUBR_INIT_DEVICE_LIB_ERROR;
	}
	// vid, pid can be parsed from params
	m_handle = hid_open_path(path.c_str());
	if (NULL == m_handle) {
		return JUBR_CONNECT_DEVICE_ERROR;
	}
	_path = path;
	return JUBR_OK;
}

JUB_RV JubiterHidDevice::disconnect() {
    firstCmd = false;

    if (NULL != m_handle) {
        hid_close(m_handle);
        m_handle = NULL;

        if (0 != hid_exit()) {
            return JUBR_ERROR;
        }
    }

    return JUBR_OK;
}

JUB_RV JubiterHidDevice::sendData(IN JUB_BYTE_CPTR sendData,
                                  IN JUB_ULONG sendLen,
                                  OUT JUB_BYTE_PTR pRetData,
                                  INOUT JUB_ULONG_PTR pulRetLen,
                                  IN JUB_ULONG ulMiliSecondTimeout) {
    // auto connect
    if (NULL == m_handle) {
        JUB_VERIFY_RV(connect(_path));
    }

#ifdef _DEBUG
	uchar_vector log(sendData, sendData+ sendLen);
	std::cout << log.getHex() << std::endl << std::endl;
#endif



    // fix cid
    JUB_BYTE cid[] = {0x00, 0x00, 0x00, 0x01};
    if (firstCmd) {
        firstCmd = false;

        std::vector<JUB_BYTE> bufferSend;

        bufferSend.push_back(0x00);
        bufferSend.push_back(cid[0]);
        bufferSend.push_back(cid[1]);
        bufferSend.push_back(cid[2]);
        bufferSend.push_back(cid[3]);
        bufferSend.push_back(0x86);
        bufferSend.push_back(0x00);
        bufferSend.push_back(0x00);

        int res = 0;
        res = write(bufferSend.data(), bufferSend.size());
        if (res < 0) {
            disconnect();
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }

        JUB_BYTE buffer[HID_PACKET_SIZE] = {0};
        res = read(buffer, sizeof(buffer));
        if (res < 0) {
            disconnect();
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }
    }

    JUB_ULONG offset = 0;
    JUB_ULONG nPackets = 0;
    while (offset < sendLen) {
		std::vector<JUB_BYTE> bufferSend;

		bufferSend.push_back(0);
		bufferSend.push_back(cid[0]);
		bufferSend.push_back(cid[1]);
		bufferSend.push_back(cid[2]);
		bufferSend.push_back(cid[3]);

		constexpr JUB_ULONG first_pack_max_len = HID_PACKET_SIZE - HID_PACKET_HEAD_FRIST;
		constexpr JUB_ULONG next_pack_max_len = HID_PACKET_SIZE - HID_PACKET_HEAD_ROUND;
        if (0 == offset) {

			bufferSend.push_back(0x83);

            // first packet
            bufferSend.push_back((JUB_BYTE)(sendLen & 0xff00));
            bufferSend.push_back((JUB_BYTE)(sendLen & 0x00ff));

            if (sendLen < first_pack_max_len) {
                bufferSend.insert(bufferSend.end(), sendData,
                                  sendData + sendLen);
                offset = sendLen;
            } else {
                bufferSend.insert(
                    bufferSend.end(), sendData,
                    sendData + (first_pack_max_len));
                offset = first_pack_max_len;
            }
        } else {
            bufferSend.push_back((JUB_BYTE)(nPackets++));

            if (sendLen - offset < next_pack_max_len) {
                bufferSend.insert(bufferSend.end(), sendData + offset,
                                  sendData + sendLen);
                offset += sendLen - offset;
            } else {
                bufferSend.insert(
                    bufferSend.end(), sendData + offset,
                    sendData + offset + next_pack_max_len);
                offset += next_pack_max_len;
            }
        }

        int res = 0;
        res = write(bufferSend.data(), bufferSend.size());
        if (res < 0) {
            disconnect();
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }
    }

    std::vector<JUB_BYTE> vRecv;
    offset = 0;
    JUB_ULONG playloadSize = 0;
    JUB_BYTE buff[HID_PACKET_SIZE] = {0};

    while (true) {
        memset(buff, 0, sizeof(buff));

        // here hid lib skip the first 0x00
        int res =
            hid_read_timeout(m_handle, buff, sizeof(buff), ulMiliSecondTimeout);
        if (res <= 0) {
            disconnect();
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }

        if (0 == offset) {
            // first packet
            playloadSize = (buff[5] << 8) + buff[6];
            if (*pulRetLen < playloadSize) {
                return JUBR_BUFFER_TOO_SMALL;
            }

            vRecv.resize(playloadSize, 0);

            if (playloadSize < HID_PACKET_GNU_SIZE - HID_PACKET_RHEAD_FRIST) {
                memcpy(&vRecv[offset], buff + HID_PACKET_RHEAD_FRIST,
                       playloadSize);
                offset += playloadSize;
            } else {
                memcpy(&vRecv[offset], buff + HID_PACKET_RHEAD_FRIST,
                       HID_PACKET_GNU_SIZE - HID_PACKET_RHEAD_FRIST);
                offset += HID_PACKET_GNU_SIZE - HID_PACKET_RHEAD_FRIST;
            }
        } else {
            if (playloadSize - offset <
                HID_PACKET_GNU_SIZE - HID_PACKET_RHEAD_ROUND) {
                memcpy(&vRecv[offset], buff + HID_PACKET_RHEAD_ROUND,
                       playloadSize - offset);
                offset += playloadSize - offset;
            } else {
                memcpy(&vRecv[offset], buff + HID_PACKET_RHEAD_ROUND,
                       HID_PACKET_GNU_SIZE - HID_PACKET_RHEAD_ROUND);
                offset += HID_PACKET_GNU_SIZE - HID_PACKET_RHEAD_ROUND;
            }
        }

        if (offset >= playloadSize) {
            *pulRetLen = playloadSize;
            memcpy(pRetData, vRecv.data(), *pulRetLen);

            break;
        }
    }

    return JUBR_OK;
    /*
        HID_3KHN_PACK sendHidPacket;
        HID_3KHN_PACK_READ_64 recvHidPacket;

        int txPacketSize = sendLen;
        int txOffset = 0, offset = 0, recvLen = *pulRetLen;
        int txSize = 0;

        int rxPacketSize = 0;
        int rxOffset = 0;
        int rxSize = 0;
        JUB_ULONG dwResult = 0, dwTimeOutTimes = 0;

        while (sendLen) {
            memset(&sendHidPacket, 0, sizeof(sendHidPacket));
            //-------------------------------------------------------------------------------
            //  prepare a data package
            txSize = sendLen > 58 ? 58 : sendLen;
            if (0 == txSize) {
                break;
            }
            sendHidPacket.uc0PacketSizeH = txPacketSize >> 8;
            sendHidPacket.uc1PacketSizeL = txPacketSize;
            sendHidPacket.uc2BlockOffsetH = txOffset >> 8;
            sendHidPacket.uc3BlockOffsetL = txOffset;
            sendHidPacket.uc4BlockSizeH = txSize >> 8;
            sendHidPacket.uc5BlockSizeL = txSize;
            memmove(sendHidPacket.uc6Buffer, sendData, txSize);
            txOffset += txSize;
            sendData += txSize;
            sendLen -= txSize;
            //-------------------------------------------------------------------------------
            //  send this package
            sendHidPacket.id = 0;
            // ESLOG_INF(("send writeFile sendLen = %d\n",sendLen));

            if (!write((JUB_BYTE*)(&sendHidPacket), sizeof(HID_3KHN_PACK))) {
                            disconnect();
                return JUBR_TRANSMIT_DEVICE_ERROR;
            } else {
            }
        }

        while (1) {
            memset(&recvHidPacket, 0, sizeof(recvHidPacket));
            int res = 0;
            res = read((JUB_BYTE*)&recvHidPacket,
    sizeof(HID_3KHN_PACK_READ_64)); if (res < 0) { disconnect(); return
    JUBR_TRANSMIT_DEVICE_ERROR; } else { if (0 == recvHidPacket.uc0PacketSizeH
    && 0 == recvHidPacket.uc1PacketSizeL) { #if (defined(WIN32) ||
    defined(_WIN32)) Sleep(100); #else usleep(100 * 1000); #endif } else {
                    rxPacketSize = recvHidPacket.uc0PacketSizeH * 256 +
                                   recvHidPacket.uc1PacketSizeL;
                    rxOffset = recvHidPacket.uc2BlockOffsetH * 256 +
                               recvHidPacket.uc3BlockOffsetL;
                    rxSize = recvHidPacket.uc4BlockSizeH * 256 +
                             recvHidPacket.uc5BlockSizeL;
                    memmove(pRetData + rxOffset, recvHidPacket.uc6Buffer,
    rxSize); if (rxOffset + rxSize == rxPacketSize) { *pulRetLen = rxPacketSize;
                        break;
                    }
                }
            }
        }*/
}

int JubiterHidDevice::write(const unsigned char* data, size_t length) {
    return hid_write(m_handle, data, length);
}

int JubiterHidDevice::read(unsigned char* data, size_t length) {
    return hid_read(m_handle, data, length);
}

}  // namespace jub

#endif  // _WIN32

#endif //HID_MODE
