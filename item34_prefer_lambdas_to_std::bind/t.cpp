// Intro to chrono
// -- A precision-neutral library for time and data
//
// Clock:
// std::chrono::system_clock: operating system clock, this can be modified by system user
//                            so it's not steady
//
// std::chrono::steady_clock: goes at a uniform rate
//
// std::chrono::high_resolution_clock: smallest possible tick period. In practice, this is
//                                     typedef of system_clock or steay_clock, you can use
//                                     it as a base class for your implementation.         
//
// Duration:
// std::chrono::duration<>:  represents time duration
//                           duration<int, std::ratio<1,1>  number of seconds stored in int
//                           duration<doube, std::ratio<60,1>  number of minutes stored in double
//      nanoseconds, microseconds, milliseconds, seconds, munites, hours are pre defined duration.
//
//
// Time point:
// std::chrono::time_point<clock, duration>: represents a point of time
//      UTC -- epoch of a clock
//      
//
//

// std::bind is the C++11 successor to C++98's std::bind1st and std::bind2nd

#include <iostream>
#include <chrono>
#include <ctime>
#include <functional>

using Time = std::chrono::steady_clock::time_point;
using Duration = std::chrono::steady_clock::duration;

enum class Sound {
    Beep,
    Siren,
    Whistle,
};

void setAlarm(const Time& t, const Sound& s, const Duration& d)
{
    using namespace std::chrono;

    auto timestamp = system_clock::to_time_t(system_clock::now() + 
            duration_cast<system_clock::duration>(t - steady_clock::now()));

    auto timestamp_cstr = std::ctime(&timestamp);
    timestamp_cstr[strlen(timestamp_cstr) - 1] = '\0';

    auto duration_in_second = duration_cast<seconds>(d).count();
    switch(s) {
        case Sound::Beep:
            std::cout << "Alarm start at: "
                      << timestamp_cstr
                      << " Beep for "
                      << duration_in_second
                      << "s\n";
            break;
        case Sound::Siren:
            std::cout << "Alarm start at: "
                      << timestamp_cstr
                      << " Siren for "
                      << duration_in_second
                      << "s\n";
            break;
        case Sound::Whistle:
            std::cout << "Alarm start at: "
                      << timestamp_cstr
                      << " Whistle for "
                      << duration_in_second
                      << "s\n";
            break;
        default:
            std::cout << "unknown sound...\n";
    }
}

int main()
{
    using namespace std::chrono;
    using namespace std::literals;
    // normal call
    setAlarm(steady_clock::now(), Sound::Beep, 10s);

    // decide to set the alarm one hour later with duration 30s using lambda
    auto setSoundL = [](Sound s) {
        setAlarm(steady_clock::now() + 1h, s, 30s);
    };
    setSoundL(Sound::Siren);

    // decide to set the alarm one hour later with duration 30s using std::bind
    // note#1: "steady_clock::now() + 1h" is passed as an argument to std::bind
    // not to setAlarm, that means that the expression will be evaluated when
    // std::bind is called, and the timing resulting from that expression will
    // be stored inside the resulting bind object. As a consequence, the alarm
    // will be set to go off after the call to std::bind, not an hour after the
    // call to setAlarm!
    using namespace std::placeholders;
    auto setSoundB = std::bind(setAlarm,
                               steady_clock::now() + 1h, // note#1
                               _1,
                               30s);
    setSoundB(Sound::Siren);

    auto setSoundB_fix = std::bind(setAlarm,
                                   std::bind(std::plus<>(), steady_clock::now(),  1h), // to fix above issue
                                   _1,
                                   30s);
    setSoundB_fix(Sound::Siren);
}
