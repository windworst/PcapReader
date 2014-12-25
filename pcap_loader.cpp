#include "pcap_loader.h"

pcap_loader::pcap_loader(const char* path)
{
    this->load_file(path);
}

pcap_loader::~pcap_loader()
{
    this->close();
}

int pcap_loader::status()
{
    if ( this->file_pcap == NULL )
    {
        return OPEN_FILE_ERROR;
    }
    else if ( head.magic != MAGIC_NUMBER )
    {
        return FILE_TYPE_ERROR;
    }
    return SUCCESS;
}

pcap_file_head pcap_loader::get_head()
{
    return this->head;
}

int pcap_loader::get_packs_count()
{
    return this->data_list.size();
}

pcap_data_info pcap_loader::get_pack(int index)
{
    return this->data_list.at(index);
}

int64_t pcap_loader::read_from(void *buf, int64_t len, int64_t offset = 0)
{
    fseek( this->file_pcap, offset, SEEK_SET );
    return fread( (char*)buf, 1, len, this->file_pcap );
}

int64_t pcap_loader::read_from_pack(pcap_data_info &info, void *buf, int64_t len, int64_t offset)
{
    return this->read_from( buf, len, info.offset + offset );
}

void pcap_loader::load_file(const char *path)
{
    if( path == NULL) return;
    this->file_pcap = fopen( path, "rb" );
    if( this->file_pcap == NULL )
    {
        return;
    }

    int64_t current_offset = 0;

    /* 处理文件头 */
    current_offset += fread( &head, 1, sizeof( head ), this->file_pcap );

    if(head.magic != MAGIC_NUMBER)
    {
        return;
    }

    /* 处理数据包 */
    pcap_data_info item;
    int64_t nread = 0;
    while( ( nread = fread(&item.data, 1, sizeof(item.data), this->file_pcap ) )  == sizeof(item.data) )
    {
      current_offset += nread;
      item.offset = current_offset;
      current_offset += item.data.pack_length;

      this->data_list.push_back( item );
      fseek( this->file_pcap, item.data.pack_length, SEEK_CUR );

    }
}

void pcap_loader::close()
{
    if( file_pcap!=NULL )
    {
        fclose( file_pcap );
        file_pcap = NULL;
    }
    data_list.clear();
    this->head.magic = 0;
}
