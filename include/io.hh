#ifndef _IO_HH
#define _IO_HH

#include <cstdio>

#include <string>

namespace io {
  class File {
    public:
      File(void) = delete;

      File(FILE *fd, std::string fname, std::string md);
      ~File(void);

      // Open reopens a file with "+a".
      void Open(void);
      // Close closes the stream, but can be reopened with Open.
      void Close(void);

      // Write writes a string to a file according to its set mode.
      void Write(const std::string &text);

      // Clear erases the file's content.
      void Clear(void);

    private:
      FILE *f;
      std::string filename;
      std::string mode;
  };

  // Open returns a file.
  File* Open(std::string filename, std::string mode = "+a");

  // Closes a file.
  void Close(File *f);

  // Create creates a given directory. If dir already exists, does nothing.
  void Mkdir(std::string dir);
}

#endif
