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

      // Open reopens a file with the given mode.
      void Open(std::string md);

      // Close closes the stream, but can be reopened with Open.
      void Close(void);

      // Write writes a string to a file according to its set mode.
      void Write(const std::string &text);

      // WriteLine writes a line to a file according to its set mode.
      void WriteLine(const std::string &text);

      // Prepend writes a line to the beginning of the file.
      void Prepend(const std::string &text);

      // ReplaceHeader replaces the first line of a file with the given line.
      void ReplaceHeader(const std::string &text);

      // Read reads a line of a string. Returns the number of characters read or
      // std::char_traits<char>::eof if end-of-file. Requires POSIX.
      int Read(std::string *line);

      // Touch checks if a file exists. If it does not, create it. Contrary to sh's touch, it does
      // not alter the timestamp on the file. Returns whether the file existed already.
      bool Touch(void);

      // Clear erases the file's content.
      void Clear(void);

    private:
      FILE *f;
      std::string filename;
      std::string mode;
  };

  // Open returns a file.
  File* Open(std::string filename, std::string mode = "+a", bool open = true);

  // Closes a file.
  void Close(File *f);

  // Create creates a given directory. If dir already exists, does nothing.
  void Mkdir(std::string dir);
}

#endif
