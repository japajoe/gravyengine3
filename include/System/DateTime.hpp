#ifndef GRAVYENGINE_DATETIME_HPP
#define GRAVYENGINE_DATETIME_HPP

#include <chrono>
#include <iomanip>
#include <sstream>

namespace GravyEngine
{
    typedef std::chrono::system_clock::time_point TimeStamp;

    class DateTime
    {
    private:
        TimeStamp timestamp;
    public:
        /// @brief Initializes a new DateTime instance and sets the timestamp to 'now'
        DateTime();

        /// @brief Initializes a new DateTime instance with given timestamp
        /// @param timestamp The timestamp to set
        DateTime(const TimeStamp& timestamp);
        
        /// @brief Gets the date component of this instance
        /// @return The date component of this instance
        TimeStamp Date();

        /// @brief Gets the day of the month represented by this instance
        /// @return The day of the month
        int Day();

        /// @brief Gets the day of the week represented by this instance
        /// @return The day of the week
        int DayOfWeek();

        /// @brief Gets the day of the year represented by this instance
        /// @return The day of the year
        int DayOfYear();

        /// @brief Gets the hour component of the date represented by this instance
        /// @return The hour of the day
        int Hour();    

        /// @brief Gets the minute component of the date represented by this instance
        /// @return The minute of the hour
        int Minute();

        /// @brief Gets the month component of the date represented by this instance
        /// @return The month of the year
        int Month();

        /// @brief Adds milliseconds to this DateTime instance
        /// @param milliseconds The number of milliseconds to add
        /// @return A new DateTime that adds the specified number of milliseconds to the value of this instance
        DateTime AddMilliseconds(int64_t milliseconds);

        /// @brief Adds seconds to this DateTime instance
        /// @param seconds The number of seconds to add
        /// @return A new DateTime that adds the specified number of seconds to the value of this instance
        DateTime AddSeconds(int64_t seconds);

        /// @brief Adds minutes to this DateTime instance
        /// @param minutes The number of minutes to hour
        /// @return A new DateTime that adds the specified number of minutes to the value of this instance
        DateTime AddMinutes(int64_t minutes);

        /// @brief Adds hours to this DateTime instance
        /// @param hours The number of hours to add
        /// @return A new DateTime that adds the specified number of hours to the value of this instance
        DateTime AddHours(int64_t hours);

        /// @brief Adds days to this DateTime instance
        /// @param days The number of days to add
        /// @return A new DateTime that adds the specified number of days to the value of this instance
        DateTime AddDays(int64_t days);

        /// @brief Gets a DateTime object that is set to the current date and time on this computer, expressed as the local time
        /// @return The local time
        static DateTime Now();

        /// @brief Gets the seconds component of the date represented by this instance
        /// @return The second of the minute
        int Second();

        /// @brief Gets the year component of the date represented by this instance.
        /// @return The year 
        int Year();

        /// @brief Gets the time in seconds since the Unix epoch
        /// @return The time in seconds
        int64_t TimeSinceEpochInSeconds();

        /// @brief Gets the time in milliseconds since the Unix epoch
        /// @return The time in milliseconds
        int64_t TimeSinceEpochInMilliseconds();

        std::string FormattedTimestamp() const;

        bool operator==(const DateTime& other) const
        {
            return timestamp == other.timestamp;
        }

        bool operator!=(const DateTime& other) const
        {
            return timestamp != other.timestamp;
        }

        bool operator>(const DateTime& other) const
        {
            return timestamp > other.timestamp;
        }

        bool operator>=(const DateTime& other) const
        {
            return timestamp >= other.timestamp;
        }

        bool operator<(const DateTime& other) const
        {
            return timestamp < other.timestamp;
        }

        bool operator<=(const DateTime& other) const
        {
            return timestamp <= other.timestamp;
        }
    };
};

#endif