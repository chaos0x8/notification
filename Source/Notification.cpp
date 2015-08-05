/*!
    \author <https://github.com/chaos0x8>
    \copyright
    Copyright (c) 2015, <https://github.com/chaos0x8>

    \copyright
    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    \copyright
    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <Notification.hpp>
#include <stdexcept>
#include <cassert>

Notification::Notification(const std::string& name)
    : name(name)
{
    assert(name.size());

    notify_init(name.c_str());
}

void Notification::show()
{
    assert(note == nullptr);

    note = notify_notification_new(name.c_str(), content.c_str(), icon.c_str());
    notify_notification_set_timeout(note, timeout.count());
    notify_notification_set_category(note, "Testing Notifications");
    notify_notification_set_urgency(note, NOTIFY_URGENCY_NORMAL);

    GError* error = nullptr;
    notify_notification_show(note, &error);

    if (error != nullptr)
    {
        throw std::runtime_error("error during 'notify_notification_show");
    }
}

const std::string Notification::ICON_INFO = "info";
