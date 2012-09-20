#ifndef __FS_OWNER_HPP
#define __FS_OWNER_HPP

#include <string>
#include <ostream>
#include <boost/serialization/access.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/string.hpp>
#include <boost/unordered_map.hpp>
#include "boost/serialization/unordered_map.hpp"
#include "acl/types.hpp"
#include "fs/path.hpp"
#include "fs/filelock.hpp"

namespace fs
{

class Owner
{
  uid_t uid;
  gid_t gid;

  template <class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & uid;
    ar & gid;
    
    (void) version;
  }

  Owner() : uid(0), gid(0) { }
  
public:
  Owner(uid_t uid, gid_t gid) : uid(uid), gid(gid) { }
  
  uid_t UID() const { return uid; }
  gid_t GID() const { return gid; }

  friend class boost::serialization::access;
  friend Owner GetOwner(const Path& path);
};
  
class OwnerEntry
{
  std::string name;
  Owner owner;

  template <class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & name;
    ar & owner;
    
    (void) version;
  }

public:
  OwnerEntry() : owner(-1, -1) { }

  OwnerEntry(const std::string& name, const Owner& owner) :
    name(name), owner(owner) { }
    
  const std::string& Name() const { return name; }
  const Owner& GetOwner() const { return owner; }
  
  void Chown(const Owner& owner) { this->owner = owner; }
  void Rename(const std::string& name) { this->name = name; }

  friend class boost::serialization::access;
};

struct OwnerFile
{
  std::string parent;
  std::string ownerFile;
  
  boost::unordered_map<std::string, OwnerEntry> entries;
  
  void Create(const std::string& name, const Owner& owner);

  template <class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & entries;
    
    (void) version;
  }
  
  static const std::string ownerFilename;
  
  bool InnerLoad(FileLockPtr& lock);
  bool InnerSave(FileLockPtr& lock);
  
public:
  OwnerFile(const Path& parent) :
    parent(parent), ownerFile(parent / ownerFilename) { }
  
  void Chown(const std::string& name, const Owner& owner);
  void Rename(const std::string& oldName, const std::string& newName);
  void Delete(const std::string& name);
  bool Exists(const std::string& name) const;
  const Owner& GetOwner(const std::string& name) const;

  bool Load(FileLockPtr& lock);
  bool Load();
  bool Save(FileLockPtr& lock);
  bool Save();
  
  friend class boost::serialization::access;
};

class OwnerModify
{
  Path path;
  Path parent;
  Path name;
  OwnerFile ownerFile;
  
public:
  OwnerModify(const Path& path);
  
  void Chown(const Owner& owner);
  void Rename(const Path& newName);
  void Delete();
};

Owner GetOwner(const Path& path);

std::ostream& operator<<(std::ostream& os, const Owner& owner);

} /* fs namespace */



#endif