#ifndef PCAPLOADER_H
#define PCAPLOADER_H

#define __USE_FILE_OFFSET64
#define __USE_LARGEFILE64
#define _LARGEFILE64_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <vector>
using std::vector;

#define MAGIC_NUMBER 0XA1B2C3D4

struct linktype_str
{
  int32_t linktype;
  const char* const type_str;
};

#pragma pack(push)
#pragma pack(1)
struct pcap_file_head //文件头结构
{
  int32_t magic; // 默认: 0XA1B2C3D4
  int16_t minor_version,magor_version; //主次版本号
  int32_t timezone; //当地标准时间
  int32_t sigflags; //时间戳精度
  int32_t snaplen; //最大储存长度
  int32_t linktype; //链路类型

  const char* get_linktype_str()
  {
    static linktype_str linktype_list[] =
    {
      {0,   "BSD loopback devices, except for later OpenBSD" },
      {1,   "Ethernet, and Linux loopback devices"},
      {6,   "802.5 Token Ring"},
      {7,   "ARCnet"},
      {8,   "SLIP"},
      {9,   "PPP"},
      {10,  "FDDI"},
      {100, "LLC/SNAP-encapsulated ATM"},
      {101, "raw IP”, with no link"},
      {102, "BSD/OS SLIP"},
      {103, "BSD/OS PPP"},
      {104, "Cisco HDLC"},
      {105, "802.11"},
      {108, "later OpenBSD loopback devices (with the AF_value in network byte order)"},
      {113, "special Linux “cooked” capture"},
      {114, "LocalTalk"},
    };

    const char* linktype_str = "[unknown linktype]";
    int i;
    for( i=0; i < sizeof( linktype_list ) / sizeof( *linktype_list ) ; ++i )
    {
      if ( linktype == linktype_list[i].linktype )
      {
        linktype_str = linktype_list[i].type_str;
        break;
      }
    }
    return linktype_str;
  }
};

struct pcap_data_head //数据包头结构
{
  int32_t timestamp_ms; //时间戳 毫秒
  int32_t timestamp_us; //时间戳 微秒
  int32_t pack_length; //抓包长度
  int32_t real_length; //实际抓包长度
};
#pragma pack(pop)

struct pcap_data_info
{
    pcap_data_head data;
    int64_t offset;
    int32_t length()
    {
        return data.real_length;
    }
};

class pcap_loader
{
public:
    pcap_loader(const char* path);
    ~pcap_loader();

    enum STATUS{ SUCCESS=0, OPEN_FILE_ERROR, FILE_TYPE_ERROR };
    int status();
    pcap_file_head get_head();
    int get_packs_count();
    pcap_data_info get_pack(int index);
    int64_t read_from(void* buf, int64_t len, int64_t offset);
    int64_t read_from_pack(pcap_data_info& info, void* buf, int64_t len, int64_t offset = 0);

protected:
    void load_file(const char* path);

private:
    FILE* file_pcap;
    pcap_file_head head;
    vector < pcap_data_info > data_list;
};

#endif // PCAPLOADER_H
