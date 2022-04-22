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

#ifndef CppMicroServices_EM_EMConstants_hpp
#define CppMicroServices_EM_EMConstants_hpp

#include "cppmicroservices/em/EMExport.h"

#include <string>

namespace cppmicroservices {
namespace em {

/**
 \defgroup gr_emconstants Constants
 \brief Groups cppmicroservices::em::Constants related symbols.
 */

/**
 \ingroup gr_emconstants
 Defines standard names for EventAdmin property constants. 
 */
namespace Constants {
/**
 * Event property identifying the bundle where the event was sent from. The value of this property
 * is of type <code>cppmicroservices::Bundle</code>.
 */
US_em_EXPORT extern const std::string BUNDLE;

/**
 * Event property identifying the bundle id of the bundle where the event was sent from. The value
 * of this property is of type <code>unsigned int</code>.
 */
US_em_EXPORT extern const std::string BUNDLE_ID;

//US_em_EXPORT extern const std::string BUNDLE_SIGNER;

/**
 * Event property identifying the symbolic name of the bundle where the event was sent from. The
 * value of this property is of type <code>std::string</code>.
 */
US_em_EXPORT extern const std::string BUNDLE_SYMBOLICNAME;

/**
 * Event property identifying the version of the bundle where the event was sent from. The value of
 * this property is of type <code>std::string</code>.
 */
US_em_EXPORT extern const std::string BUNDLE_VERSION;

/**
 * Event property identifying whether or not the event should be delivered asychronously in order.
 * The value of this property is of type <code>bool</code>.
 *
 * This property is mututally exclusive with \c DELIVERY_ASYNC_UNORDERED . If both are specified,
 * this value will take precedence.
 * 
 * If neither \c DELIVERY_ASYNC_ORDERED and \c DELIVERY_ASYNC_UNORDERED are specified, events
 * will be delivered in order.
 */
US_em_EXPORT extern const std::string DELIVERY_ASYNC_ORDERED;

/**
 * Event property identifying whether or not the event should be delivered asychronously without
 * regard for order. The value of this property is of type <code>bool</code>.
 *
 * This property is mututally exclusive with \c DELIVERY_ASYNC_ORDERED . If both are specified, 
 * \c DELIVERY_ASYNC_UNORDERED will take precedence.
 * 
 * If neither \c DELIVERY_ASYNC_ORDERED and \c DELIVERY_ASYNC_UNORDERED are specified, events
 * will be delivered in order.
 */
US_em_EXPORT extern const std::string DELIVERY_ASYNC_UNORDERED;

/**
 * Event property containing the forwarded \c EMEvent object. This can be used when rebroadcasting
 * the event. The value of this property is of type <code>EMEvent</code>
 */
US_em_EXPORT extern const std::string EVENT;

//US_em_EXPORT extern const std::string EVENT_ADMIN_IMPLEMENTATION;

//US_em_EXPORT extern const std::string EVENT_ADMIN_SPECIFICATION_VERSION;

/**
 * A service registration property which specifies how events should be delivered. The value is of
 * type <code>std::string</code> and should contain one of the following values:
 * "DELIVERY_ASYNC_ORDERED", "DELIVERY_ASYNC_UNORDERED".
 */
US_em_EXPORT extern const std::string EVENT_DELIVERY;

/**
 * A service registration property which specifies a \c LDAPFilter for filtering what types of
 * events the \c EventHandler should receive.
 *
 * This is an optional property. 
 *
 * If the property is specified, the handler will only receive events that match the topic and the
 * filter. If not specified, the handler will receive all events matching the specified topic.
 *
 * The value of this property is of type <code>LDAPFilter</code>
 */
US_em_EXPORT extern const std::string EVENT_FILTER;

/**
 * A service registration property for specifying which topics the \c EventHandler should receive.
 * 
 * All \c EventHandler services must be registered with this property.
 * 
 * Topics take the following form:
 *   topic-description := '*' | topic ( '/\*' )?
 *   topic := token ( '/' token )*
 * 
 * If no topic is provided, the associated \c EventHandler will not receive any events.
 * 
 * The value of this property is of type <code>std::vector<std::string></code>
 */
US_em_EXPORT extern const std::string EVENT_TOPIC;

/**
 * Event property containing the exception object if one was thrown. The value of this property is
 * of type equal to that of the string representation of \c EXCEPTION_CLASS .
 */
US_em_EXPORT extern const std::string EXCEPTION;

/**
 * Event property containing the stringified type of the exception that was thrown (if actually
 * thrown). The value of this property is of type <code>std::string</code>
 */
US_em_EXPORT extern const std::string EXCEPTION_CLASS;

/**
 * Event property containing the exception message of the thrown exception (if one was thrown). The
 * value of this property is of type <code>std::string</code> and should be identical to the value
 * returned by <code>std::exception::what()</code>.
 */
US_em_EXPORT extern const std::string EXCEPTION_MESSAGE;

/**
 * Event property containing a human-readable message to accompany the event. The value of this
 * property is of type <code>std::string</code>.
 */
US_em_EXPORT extern const std::string MESSAGE;

/**
 * Event property containing a service reference of your choosing. The value of this property is of
 * type <code>cppmicroservices::ServiceReferenceU</code>.
 */
US_em_EXPORT extern const std::string SERVICE;

/**
 * Event property containing the id of the provided service reference in \c SERVICE . The value of
 * this property is of type <code>unsigned int</code>.
 */
US_em_EXPORT extern const std::string SERVICE_ID;

/**
 * Event property containing the objectclass of the provided service reference in \c SERVICE . The
 * value of this property is of type <code>std::string</code>.
 */
US_em_EXPORT extern const std::string OBJECTCLASS;

/**
 * Event property containing the provided service's persistent ID. The value of this property is of
 * type <code>std::string</code>.
 */
US_em_EXPORT extern const std::string SERVICE_PID;

/**
 * Event property containing the time at which the event occurred. The value of this property is of
 * type <code>std::chrono::high_resolution_clock::time_point</code>.
 */
US_em_EXPORT extern const std::string TIMESTAMP;
}
}
}

#endif
