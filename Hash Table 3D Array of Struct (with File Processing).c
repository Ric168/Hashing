#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX 52
#define MAX_LENGTH_NIM 20
#define MAX_LENGTH_NAMA 100

typedef struct Mahasiswa
{
    char nim[MAX_LENGTH_NIM];
    char nama[MAX_LENGTH_NAMA];
    struct Mahasiswa *kanan;
} Mahasiswa;

Mahasiswa *HashTable[MAX][MAX][MAX];

typedef struct HashTableIndices
{
    int index1, index2, index3;
} HashTableIndices;

// 'AAA'
// [0][0][0] Mahasiswa

// 'AAB'
// [0][0][1] Mahasiswa -> Mahasiswa -> Mahasiswa
// [0][0][2] Mahasiswa -> Mahasiswa -> Mahasiswa

// 'AAZ'
// [0][0][25] Mahasiswa -> Mahasiswa -> Mahasiswa
// 'AAa'
// [0][0][26]

// ...
// 'zzz'
// [52][52][52] Mahasiswa -> Mahasiswa -> Mahasiswa

int hash_function(char input)
{
    return input - (input < 97 ? 65 : 71);
}
struct HashTableIndices cariHashTableIndex(char *nama)
{
    struct HashTableIndices ret;
    ret.index1 = hash_function(nama[0]);
    ret.index2 = hash_function(nama[1]);
    ret.index3 = hash_function(nama[2]);
    return ret;
}

void createNewNode(char *nim, char *nama)
{
    Mahasiswa *newNode = (struct Mahasiswa *)malloc(sizeof(struct Mahasiswa));
    strcpy(newNode->nim, nim);
    strcpy(newNode->nama, nama);

    struct HashTableIndices hti = cariHashTableIndex(nama);
    newNode->kanan = HashTable[hti.index1][hti.index2][hti.index3];
    HashTable[hti.index1][hti.index2][hti.index3] = newNode;
}

void bacaFile()
{
    FILE *read = fopen("datamhs.txt", "r");
    char nim[MAX_LENGTH_NIM];
    char nama[MAX_LENGTH_NAMA];

    while (!feof(read))
    {
        fscanf(read, "%[^#]#%[^\n] ", nim, nama);
        createNewNode(nim, nama);
    }
    fclose(read);
}

Mahasiswa *hapusNode(Mahasiswa *dihapus)
{
    if (dihapus == NULL)
        return NULL;

    Mahasiswa *newNode = dihapus->kanan;
    dihapus->kanan = NULL;
    char nim[MAX_LENGTH_NIM];
    char nama[MAX_LENGTH_NAMA];
    strcpy(nim, dihapus->nim);
    strcpy(nama, dihapus->nama);

    printf("Mahasiswa dengan nama %s nim %s berhasil dihapus dari memory!\n", nama, nim);
    free(dihapus);
    dihapus = NULL;
    return newNode;
}

void *hapusNodeByName(char *nama)
{
    struct HashTableIndices hti = cariHashTableIndex(nama);
    Mahasiswa *head = HashTable[hti.index1][hti.index2][hti.index3];
    Mahasiswa *prev = NULL;
    bool ketemu = false;
    while (head != NULL)
    {
        if (strcmp(nama, head->nama) == 0)
        {
            if (prev == NULL)
            {
                HashTable[hti.index1][hti.index2][hti.index3] = head->kanan;
            }
            else
            {
                prev->kanan = head->kanan;
            }
            head->kanan = NULL;
            free(head);
            head = NULL;
            ketemu = true;
            break;
        }
        prev = head;
        head = head->kanan;
    }

    if (!ketemu)
    {
        printf("Mahasiswa dengan nama %s tidak ada.\n", nama);
    }
    else
    {
        printf("Mahasiswa dengan nama %s berhasil dihapus!\n", nama);
    }
}

void hapusSemuaLinkedList(Mahasiswa *headMahasiswa)
{
    while (headMahasiswa != NULL)
    {
        headMahasiswa = hapusNode(headMahasiswa);
    }
    headMahasiswa = NULL;
}

void inisialisasi()
{
    for (int iArr1 = 0; iArr1 < MAX; iArr1++)
    {
        for (int iArr2 = 0; iArr2 < MAX; iArr2++)
        {
            for (int iArr3 = 0; iArr3 < MAX; iArr3++)
            {
                HashTable[iArr1][iArr2][iArr3] = NULL;
            }
        }
    }
}

void hapusSemuaArray()
{
    for (int iArr1 = 0; iArr1 < MAX; iArr1++)
    {
        for (int iArr2 = 0; iArr2 < MAX; iArr2++)
        {
            for (int iArr3 = 0; iArr3 < MAX; iArr3++)
            {
                hapusSemuaLinkedList(HashTable[iArr1][iArr2][iArr3]);
            }
        }
    }
}



void tampilkanMahasiswa(Mahasiswa *head)
{
    int ke = 0;
    while (head != NULL)
    {
        printf("Mahasiswa #%d\n", ++ke);
        printf("Nama : %s\n", head->nama);
        printf("NIM : %s\n", head->nim);
        printf("--------------------------------------\n");
        head = head->kanan;
    }
}

void tampilkanSemuaArray()
{
    for (int iArr1 = 0; iArr1 < MAX; iArr1++)
    {
        for (int iArr2 = 0; iArr2 < MAX; iArr2++)
        {
            for (int iArr3 = 0; iArr3 < MAX; iArr3++)
            {
                tampilkanMahasiswa(HashTable[iArr1][iArr2][iArr3]);
            }
        }
    }
}

void tampilkanMahasiswaByName(char *nama)
{
    struct HashTableIndices hti = cariHashTableIndex(nama);
    Mahasiswa *head = HashTable[hti.index1][hti.index2][hti.index3];
    tampilkanMahasiswa(head);
}

void simpanFile()
{
    FILE *write = fopen("datamhs.txt","w");

    for (int iArr1 = 0; iArr1 < MAX; iArr1++)
    {
        for (int iArr2 = 0; iArr2 < MAX; iArr2++)
        {
            for (int iArr3 = 0; iArr3 < MAX; iArr3++)
            {
                Mahasiswa *head = HashTable[iArr1][iArr2][iArr3];
                while (head != NULL)
                {

                fprintf(write, "%d#%s\n",head->nim,head->nama);
                head = head->kanan;
                }
            }
        }
    }fclose(write);
}

int main()
{
    inisialisasi();
    bacaFile();
    int option;
    do
    {
        printf("=================================================\n");
        printf("\t\tDaftar Mahasiswa\t\t\n");
        printf("=================================================\n");
        printf("(1). Cari Mahasiswa (berdasar 3 huruf pertama)\n");
        printf("(2). Delete Mahasiswa\n");
        printf("(3). Tambah Mahasiswa\n");
        printf("(4). Data di Hash Table\n");
        printf("(0). Exit\n");
        printf("pilih : \n");
        scanf("%d", &option);
        fflush(stdin);
        switch (option)
        {
        case 1:
            printf("Masukan 3 huruf yang ingin dicari:\t");
            char namaCari[MAX_LENGTH_NAMA];
            scanf("%s", namaCari);
            fflush(stdin);
            tampilkanMahasiswaByName(namaCari);
            break;

        case 2:
            printf("Masukkan nama yang ingin dihapus:\t");
            char namaHapus[MAX_LENGTH_NAMA];
            fgets(namaHapus, MAX_LENGTH_NAMA, stdin);
            int len = strlen(namaHapus);
            if (len > 0 && namaHapus[len - 1] == '\n')
            {
                namaHapus[len - 1] = '\0';
            }
            hapusNodeByName(namaHapus);
            simpanFile();
            break;

        case 3:
            printf("Enter the mahasiswa Details\n");
            printf("NIM:\t");
            char namaTambah[MAX_LENGTH_NAMA];
            char nim[MAX_LENGTH_NIM];
            scanf("%s", nim);
            fflush(stdin);
            printf("Nama:\t");
            fgets(namaTambah, MAX_LENGTH_NAMA, stdin);
            len = strlen(namaTambah);
            if (len > 0 && namaTambah[len - 1] == '\n')
            {
                namaTambah[len - 1] = '\0';
            }
            createNewNode(nim, namaTambah);
            simpanFile();
            break;

        case 4:
            tampilkanSemuaArray();
            break;

        case 0:
            break;
        }
    } while (option != 0);
   // hapusSemuaArray();
    return 0;
}
