#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "io.hh"
#include "utils.hh"

namespace io {
  File::File(FILE *fd, std::string fname, std::string md) {
    f = fd;
    filename.assign(fname);
    mode.assign(md);
  }

  File::~File(void) {
    if (f != NULL) fclose(f);
  }

  File *Open(std::string filename, std::string mode, bool open) {
    if (!open) return new File(NULL, filename, mode);
    FILE *fd = fopen(filename.c_str(), mode.c_str());
    if (fd == NULL)
      utils::PrintfError("Could not open file [%s] with mode [%s].\n",
          filename.c_str(), mode.c_str());
    File *f = new File(fd, filename, mode);
    return f;
  }

  void File::Open(void) {
    if (f == NULL)
      f = fopen(filename.c_str(), mode.c_str());
  }


  bool File::Touch(void) {
    FILE *fd = fopen(filename.c_str(), "r");
    if (fd == NULL) {
      fd = fopen(filename.c_str(), "w");
      if (fd != NULL)
        fclose(fd);
      else
        utils::PrintfError("Could not touch file [%s].\n", filename);
      return false;
    }
    return true;
  }

  void File::Open(std::string md) {
    if (f == NULL)
      f = fopen(filename.c_str(), md.c_str());
  }

  void File::Close(void) {
    fclose(f);
    f = NULL;
  }

  void File::Write(const std::string &text) {
    fprintf(f, "%s", text.c_str());
  }

  void File::WriteLine(const std::string &text) {
    fprintf(f, "%s\n", text.c_str());
  }

  void File::Prepend(const std::string &text) {
    Close();
    Open("r");
    std::string block;
    std::string line;
    int n;
    const int END = std::char_traits<char>::eof();
    while (n != END) {
      n = Read(&line);
      block += line + "\n";
    }
    Close();
    Open("w");
    WriteLine(text);
    Write(block);
    Close();
  }

  void File::ReplaceHeader(const std::string &text) {
    Close();
    Open("r");
    std::string block;
    std::string line;
    int n;
    const int END = std::char_traits<char>::eof();
    n = Read(&line);
    while (n != END) {
      n = Read(&line);
      block += line + "\n";
    }
    Close();
    Open("w");
    WriteLine(text);
    Write(block);
    Close();
  }

  int File::Read(std::string *line) {
    const int END = std::char_traits<char>::eof();
    if (f == NULL) {
      utils::PrintfError("File stream named [%s] is closed. Try File::Open() before reading.\n",
          filename);
      return END;
    }
    char *buf = NULL;
    int n = getline(&buf, NULL, f);
    if (n == END) {
      free(buf);
      return n;
    }
    line->assign(buf);
    free(buf);
    return n;
  }

  void File::Clear() {
    freopen(NULL, "w", f);
    freopen(NULL, mode.c_str(), f);
  }

  void Close(File *f) {
    delete f;
  }

  void Mkdir(std::string dir) {
    const char *p = dir.c_str();
    struct stat st{};
    if (stat(p, &st))
      if (mkdir(p, 0700))
        utils::PrintfError("Could not create directory [%s].\n", p);
  }
}
