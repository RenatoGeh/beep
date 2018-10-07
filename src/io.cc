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

  File *Open(std::string filename, std::string mode) {
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

  void File::Close(void) {
    fclose(f);
    f = NULL;
  }

  void File::Write(const std::string &text) {
    fprintf(f, "%s", text.c_str());
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
        utils::PrintfError("Could not create directory [%s].", p);
  }
}
