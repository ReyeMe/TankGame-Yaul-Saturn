#pragma once

#include <yaul.h>

namespace Skathi
{
    /** @brief File and CD access wrapper
     */
    class Cd
    {
    private:
        /** @brief Current directory listing
         */
        inline static cdfs_filelist_t files;

    public:
        /** @brief Initialize file handling stuff
         */
        static void Initialize()
        {
            /* Load the maximum number. We have to free the allocated filelist
             * entries, but since we never exit, we don't have to */
            cdfs_filelist_entry_t * const filelist_entries = cdfs_entries_alloc(-1);
            assert(filelist_entries != NULL);

            cdfs_filelist_default_init(&Cd::files, filelist_entries, -1);
            cdfs_filelist_root_read(&Cd::files);
        }

        /** @brief Get the All files
         * @return List of files
         */
        static cdfs_filelist_t * GetAll()
        {
            return &Cd::files;
        }

        /** @brief Find file by name
         * @param name File name
         * @return File info or NULL if not found
         */
        static cdfs_filelist_entry_t * FindFileByName(const char * name)
        {
            assert(name != NULL);

            for (uint32_t entry = 0; entry < Cd::files.entries_count; entry++)
            {
                if (strcmp(name, Cd::files.entries[entry].name) == 0)
                {
                    return &Cd::files.entries[entry];
                }
            }

            return NULL;
        }

        /** @brief Change current directory
         * @param name Directory name (NULL for root directory)
         */
        static void ChangeDir(const char * name)
        {
            if (name == NULL)
            {
                cdfs_filelist_root_read(&Cd::files);
            }
            else
            {
                cdfs_filelist_entry_t * directory = Cd::FindFileByName(name);
                assert(directory->type == CDFS_ENTRY_TYPE_DIRECTORY);
                cdfs_filelist_read(&Cd::files, *directory);
            }
        }

        /** @brief Read whole file into a buffer
         * @param file File to read
         * @param buffer Target buffer
         * @param length Number of bytes to read
         * @return true File was read successfully 
         * @return false Reading file ended with error
         */
        static bool ReadFileBytes(const cdfs_filelist_entry_t * file, void * buffer, uint32_t length)
        {
            return cd_block_sectors_read(file->starting_fad, buffer, length) == 0;
        }

        /** @brief Read whole file into a buffer
         * @param file File to read
         * @param buffer Target buffer
         * @return true File was read successfully 
         * @return false Reading file ended with error
         */
        static bool ReadFile(const cdfs_filelist_entry_t * file, void * buffer)
        {
            return Cd::ReadFileBytes(file, buffer, file->size);
        }
    };
}