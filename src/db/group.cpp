#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <mongo/client/dbclient.h>
#include "db/group.hpp"
#include "acl/group.hpp"
#include "util/verify.hpp"
#include "db/serialization.hpp"
#include "db/connection.hpp"
#include "db/error.hpp"

namespace db
{

acl::GroupID Group::Create()
{
  SafeConnection conn;
  return conn.InsertAutoIncrement("groups", group, "gid");
}

void Group::SaveField(const std::string& field)
{
  NoErrorConnection conn;
  conn.SetField("groups", QUERY("gid" << group.ID()), group, field);
}

bool Group::SaveName()
{
  try
  {
    SafeConnection conn;
    conn.SetField("groups", QUERY("gid" << group.ID()), group, "name");
    return true;
  }
  catch (const db::DBError&)
  {
    return false;
  }
}

void Group::SaveDescription()
{
  SaveField("description");  
}

void Group::SaveComment()
{
  SaveField("comment");
}

void Group::SaveSlots()
{
  SaveField("slots");
}

void Group::SaveLeechSlots()
{
  SaveField("leech slots");
}

void Group::SaveAllotmentSlots()
{
  SaveField("allotment slots");
}

void Group::SaveMaxAllotmentSize()
{
  SaveField("max allotment size");
}

void Group::SaveMaxLogins()
{
  SaveField("max logins");
}

long long Group::NumMembers() const
{
  mongo::BSONArrayBuilder bab;
  bab.append(group.ID());
  
  NoErrorConnection conn;
  return conn.Count("users", BSON("$or" << 
              BSON_ARRAY(BSON("primary gid" << group.ID()) <<
                         BSON("secondary gids" << BSON("$in" << bab.arr())))));
}

void Group::Purge() const
{
  NoErrorConnection conn;
  conn.Remove("groups", QUERY("gid" << group.ID()));
}

template <> mongo::BSONObj Serialize<acl::Group>(const acl::Group& group)
{
  mongo::BSONObjBuilder bob;
  bob.append("name", group.name);
  bob.append("gid", group.id);
  bob.append("description", group.description);
  bob.append("slots", group.slots);
  bob.append("leech slots", group.leechSlots);
  bob.append("allotment slots", group.allotmentSlots);
  bob.append("max allotment size", group.maxAllotmentSize);
  bob.append("max logins", group.maxLogins);
  bob.append("comment", group.comment);
  return bob.obj();
}

template <> acl::Group Unserialize<acl::Group>(const mongo::BSONObj& obj)
{
  try
  {
    acl::Group group;
    group.id = obj["gid"].Int();
    group.name = obj["name"].String();
    group.description = obj["description"].String();
    group.comment = obj["comment"].String();
    group.slots = obj["slots"].Int();
    group.leechSlots = obj["leech slots"].Int();
    group.allotmentSlots = obj["allotment slots"].Int();
    group.maxAllotmentSize = obj["max allotment size"].Long();
    group.slots = obj["slots"].Int();
    group.maxLogins = obj["max logins"].Int();
    return group;
  }
  catch (const mongo::DBException& e)
  {
    LogException("Unserialize group", e, obj);
    throw e;
  }
}

boost::optional<acl::Group> Group::Load(acl::GroupID gid)
{
  NoErrorConnection conn;
  return conn.QueryOne<acl::Group>("groups", QUERY("gid" << gid));
}

namespace
{

template <typename T>
std::vector<T> GetGeneric(const std::string& multiStr, const mongo::BSONObj* fields)
{
  std::vector<std::string> toks;
  boost::split(toks, multiStr, boost::is_any_of(" "), boost::token_compress_on);
  
  mongo::Query query;
  if (std::find(toks.begin(), toks.end(), "*") == toks.end())
  {
    mongo::BSONArrayBuilder namesBab;
    
    for (std::string tok : toks)
    {
      if (tok[0] == '=') tok.erase(0, 1);
      namesBab.append(tok);
    }
    
    query = QUERY("name" << BSON("$in" << namesBab.arr()));
  }
  
  NoErrorConnection conn;
  return conn.QueryMulti<T>("groups", query, 0, 0, fields);
}

}

std::vector<acl::GroupID> GetGIDs(const std::string& multiStr)
{
  auto fields = BSON("gid" << 1);
  return GetGeneric<acl::GroupID>(multiStr, &fields);
}

std::vector<acl::Group> GetGroups(const std::string& multiStr)
{
  return GetGeneric<acl::Group>(multiStr, nullptr);
}

} /* acl namespace */