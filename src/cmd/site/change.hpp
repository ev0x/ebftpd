#ifndef __CMD_SITE_CHANGE_HPP
#define __CMD_SITE_CHANGE_HPP

#include <functional>
#include <string>
#include "cmd/command.hpp"

namespace cmd { namespace site
{

class CHANGECommand;

class CHANGECommand : public Command
{
  typedef std::function<void(acl::UserID)> SetFunction;
  typedef std::function<SetFunction(CHANGECommand*)> CheckFunction;

  struct SettingDef
  {
    std::string name;
    std::string aclKeyword;
    CheckFunction check;
  };
  
  std::string display;

  static const std::vector<SettingDef> settings;

  SetFunction CheckRatio();
  SetFunction CheckSectionRatio();
  SetFunction CheckWeeklyAllotment();
  SetFunction CheckHomeDir();
  SetFunction CheckFlags();
  SetFunction CheckIdleTime();
  SetFunction CheckExpires();
  SetFunction CheckNumLogins();
  SetFunction CheckTagline();
  SetFunction CheckComment();
  SetFunction CheckMaxUpSpeed();
  SetFunction CheckMaxDownSpeed();
  SetFunction CheckMaxSimUp();
  SetFunction CheckMaxSimDown();
  
  void AddFlags(acl::UserID uid, const std::string& flags);
  void DelFlags(acl::UserID uid, const std::string& flags);
  void SetFlags(acl::UserID uid, const std::string& flags);
  
  SetFunction Check();
  
public:
  CHANGECommand(ftp::Client& client, const std::string& argStr, const Args& args) :
    Command(client, client.Control(), client.Data(), argStr, args) { }

  void Execute();
};

} /* site namespace */
} /* cmd namespace */

#endif


