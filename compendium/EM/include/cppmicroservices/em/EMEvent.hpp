/*=============================================================================

 Library: CppMicroServices

 Copyright (c) The CppMicroServices developers. See the COPYRIGHT
 file at the top-level directory of this distribution and at
 https://github.com/CppMicroServices/CppMicroServices/COPYRIGHT .

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 =============================================================================*/

#ifndef CppMicroServices_EM_EMEvent_hpp
#define CppMicroServices_EM_EMEvent_hpp

#include "cppmicroservices/em/EMExport.h"

#include "cppmicroservices/AnyMap.h"
#include "cppmicroservices/LDAPFilter.h"

namespace cppmicroservices {
namespace service {
namespace em {

using EventProperties = std::unordered_map<std::string, Any>;

/**
 * \defgroup gr_emevent EMEvent
 * \brief Groups cppmicroservices::service::em::EMEvent related symbols. 
 */

/**
 * \ingroup gr_emevent 
 *
 * An implementation of the OSGi EventAdmin Event specification.
 *
 * The EMEvent is the event object that is both published and subscribed to. Any published events
 * are of type EMEvent. The event can contain various properties that allow consumers to filter out
 * specific events and perform certain operations when specific events are received.
 * 
 * Event properties are read-only and only allow for consumers to observe them. This is to prevent
 * event properties being changed by arbitrary pieces of code, ensuring the originality of the
 * published event.
 * 
 * Interactions with events occurs through the EMEvent object directly. See
 * \c cppmicroservices::em::Constants for information regarding what properties an event can have.
 */
class US_em_EXPORT EMEvent
{
public:
  EMEvent() = delete;
  EMEvent(EMEvent&&) = delete;
  EMEvent(const EMEvent&&) = delete;

  EMEvent(const EMEvent&) = default;

  virtual ~EMEvent() = default;

  /**
   * @brief Construct a new EMEvent object with the provided topic and event properties.
   * 
   * Topics take the following form:
   *   topic-description := '*' | topic ( '/\*' )?
   *   topic := token ( '/' token )*
   *   token := [A-Za-z0-9_.]
   *
   * @param topic The topic of the event.
   * @param properties The event's properties. This is immutable and a copy of the original
   * properties
   * 
   * @throws std::logic_error If the topic format is invalid
   */
  EMEvent(const std::string& topic,
          const EventProperties properties = EventProperties());

  /**
   * @brief Compares whether or not two EMEvents are equal to each other
   * 
   * EMEvents are euqal iff their topics match and their properties exactly match.
   * 
   * @param other The other EMEvent to compare against
   * @return true if the events are equal
   * @return false otherwise
   */
  bool operator==(const EMEvent& other) const;

  /**
   * @brief Compares whether or not two EMEvents are not equal to each other
   *
   * EMEvents are not equal if their topics do not match and/or their properties do not exactly
   * match.
   *
   * @param other The other EMEvent to compare against
   * @return true if the events are not equal
   * @return false otherwise
   */
  bool operator!=(const EMEvent& other) const;

  /**
   * @brief Returns whether or not the provided property is specified in the event properties.
   *
   * @param propName The peropty to search for
   * @return bool true if the property is defined, false otherwise
   */
  bool ContainsProperty(const std::string& propName) const;

  /**
   * @brief Returns the property associated with the specified property name. If the specified
   * property does not exist, an empty <code>cppmicroservices::Any</code> is returned
   * 
   * @param propName The name of the property to get
   * @return Any The value of the property if found, an empty Any if not found
   */
  const Any GetProperty(const std::string& propName) const;

  /**
   * @brief Returns the properties map containing the properties of the event.
   * 
   * @return The properties
   */
  const AnyMap GetProperties() const;

  /**
   * @brief Returns a vector of all the specified property names.
   * 
   * @return std::vector<std::string> The specified property names
   */
  std::vector<std::string> GetPropertyNames() const;

  /**
   * @brief Returns the topic of the EMEvent.
   * 
   * @return std::string The topic
   */
  std::string GetTopic() const;

  /**
   * @brief Returns whether or not the EMEvent matches against the provided \c LDAPFilter .
   *
   * @param filter The filter to match the event against
   * @return bool true if the specified filter matches this event, false otherwise
   */
  bool Matches(const LDAPFilter& filter) const;

private:
  const std::string topic;
  const AnyMap properties;
};
}
}
}

#endif
