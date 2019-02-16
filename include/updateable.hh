#ifndef _UPDATEABLE_HH
#define _UPDATEABLE_HH

class Updateable {
  public:
    virtual ~Updateable() {}
    virtual void Update(void *arg) = 0;
};

#endif
