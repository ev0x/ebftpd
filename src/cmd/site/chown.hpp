#ifndef __CMD_SITE_CHOWN_HPP
#define __CMD_SITE_CHOWN_HPP

#include "cmd/command.hpp"
#include "fs/owner.hpp"

namespace cmd { namespace site
{

class CHOWNCommand : public Command
{
  std::string patharg;
  std::string user;
  std::string group;
  fs::Owner owner;
  bool recursive;
  int dirs;
  int files;
  int failed;
  
  void Process(fs::VirtualPath pathmask);
  void ParseArgs();
  
public:
  CHOWNCommand(ftp::Client& client, const std::string& argStr, const Args& args) :
    Command(client, client.Control(), client.Data(), argStr, args),
    owner(-1, -1), recursive(false), dirs(0), files(0), failed(0) { }

  void Execute();
};

} /* site namespace */
} /* cmd namespace */

#endif
