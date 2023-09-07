#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void check_elf(unsigned char *e_ident);
void print_info(Elf64_Ehdr *header);
void close_elf(int elf);

void check_elf(unsigned char *e_ident)
{
    for (int index = 0; index < 4; index++)
    {
        if (e_ident[index] != 127 &&
            e_ident[index] != 'E' &&
            e_ident[index] != 'L' &&
            e_ident[index] != 'F')
        {
            dprintf(STDERR_FILENO, "Error: Not an ELF file\n");
            exit(98);
        }
    }
}

void print_info(Elf64_Ehdr *header)
{
    printf("ELF Header:\n");
    printf(" Magic: ");
    for (int index = 0; index < EI_NIDENT; index++)
        printf("%02x%s", header->e_ident[index], (index == EI_NIDENT - 1) ? "\n" : " ");
    printf(" Class: %s\n", (header->e_ident[EI_CLASS] == ELFCLASS32) ? "ELF32" : ((header->e_ident[EI_CLASS] == ELFCLASS64) ? "ELF64" : "none"));
    printf(" Data: %s\n", (header->e_ident[EI_DATA] == ELFDATA2LSB) ? "2's complement, little endian" : ((header->e_ident[EI_DATA] == ELFDATA2MSB) ? "2's complement, big endian" : "none"));
    printf(" Version: %d%s\n", header->e_ident[EI_VERSION], (header->e_ident[EI_VERSION] == EV_CURRENT) ? " (current)" : "");
    printf(" OS/ABI: ");
    switch (header->e_ident[EI_OSABI])
    {
        case ELFOSABI_NONE: printf("UNIX - System V\n"); break;
        case ELFOSABI_HPUX: printf("UNIX - HP-UX\n"); break;
        case ELFOSABI_NETBSD: printf("UNIX - NetBSD\n"); break;
        case ELFOSABI_LINUX: printf("UNIX - Linux\n"); break;
        case ELFOSABI_SOLARIS: printf("UNIX - Solaris\n"); break;
        case ELFOSABI_IRIX: printf("UNIX - IRIX\n"); break;
        case ELFOSABI_FREEBSD: printf("UNIX - FreeBSD\n"); break;
        case ELFOSABI_TRU64: printf("UNIX - TRU64\n"); break;
        case ELFOSABI_ARM: printf("ARM\n"); break;
        case ELFOSABI_STANDALONE: printf("Standalone App\n"); break;
        default: printf("<unknown: %x>\n", header->e_ident[EI_OSABI]);
    }
    printf(" ABI Version: %d\n", header->e_ident[EI_ABIVERSION]);
    printf(" Type: ");
    switch (header->e_type)
    {
        case ET_NONE: printf("NONE (None)\n"); break;
        case ET_REL: printf("REL (Relocatable file)\n"); break;
        case ET_EXEC: printf("EXEC (Executable file)\n"); break;
        case ET_DYN: printf("DYN (Shared object file)\n"); break;
        case ET_CORE: printf("CORE (Core file)\n"); break;
        default: printf("<unknown: %x>\n", header->e_type);
    }
    printf(" Entry point address: %#lx\n", header->e_entry);
}

void close_elf(int elf)
{
    if (close(elf) == -1)
    {
        dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", elf);
        exit(98);
    }
}

int main(int __attribute__((__unused__)) argc, char *argv[])
{
    Elf64_Ehdr *header;
    int o, r;

    o = open(argv[1], O_RDONLY);
    if (o == -1)
    {
        dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
        exit(98);
    }
    header = malloc(sizeof(Elf64_Ehdr));
    if (header == NULL)
    {
        close_elf(o);
        dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
        exit(98);
    }
    r = read(o, header, sizeof(Elf64_Ehdr));
    if (r == -1)
    {
        free(header);
        close_elf(o);
        dprintf(STDERR_FILENO, "Error: `%s`: No such file\n", argv[1]);
        exit(98);
    }

    check_elf(header->e_ident);
    print_info(header);

    free(header);
    close_elf(o);
    return (0);
}
