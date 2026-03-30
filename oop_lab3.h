#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <chrono>
#include <concepts>
#include <type_traits>
#include <algorithm>
#include <functional>
#include <random>
#include <cassert>
#include <array>
#include <sstream>

namespace ps {
    enum class enResolution {
        SECONDS,
        MILLISECONDS,
        MICROSECONDS,
        NANOSECONDS
    };

    class Timer {
    public:
        explicit Timer(enResolution resolution = enResolution::MILLISECONDS) : m_Resolution(resolution) {}

        void start() {
            m_isRunning = true;
            m_StartTimePoint = std::chrono::high_resolution_clock::now();
        }

        long long elapsed() {
            using namespace std::chrono;
            auto endTimePoint = high_resolution_clock::now();
            auto duration = endTimePoint - m_StartTimePoint;

            switch (m_Resolution) {
            case enResolution::SECONDS: return duration_cast<seconds>(duration).count();
            case enResolution::MILLISECONDS: return duration_cast<milliseconds>(duration).count();
            case enResolution::MICROSECONDS: return duration_cast<microseconds>(duration).count();
            case enResolution::NANOSECONDS: return duration_cast<nanoseconds>(duration).count();
            default: return duration.count();
            }
        }

        long long stop() {
            auto end = elapsed();
            m_isRunning = false;
            return end;
        }

        std::string get_unit_name() const {
            static constexpr const char* m_UnitName[] = {"s","ms","us","ns"};
            return m_UnitName[static_cast<int>(m_Resolution)];
        }

        std::string stop_and_get_unit_name() {
            return std::format("{0} {1}", stop(), get_unit_name());
        }

    protected:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
        enResolution m_Resolution;
        bool m_isRunning = false;
    };

    class ScopedTimer :public Timer {
    public:
        ScopedTimer(const std::string& name,
            enResolution resolution = enResolution::MILLISECONDS):
            m_Name(name),Timer(resolution) {
            start();
        }

        ~ScopedTimer() {
            if (m_isRunning) {
                std::cout << m_Name << ": " << stop_and_get_unit_name() << "\n";
            }
        }
    private:
        std::string m_Name = "";
    };
}

namespace ps {
    struct TimerConfig {
        bool enabled = false;
        enResolution resolution = enResolution::MILLISECONDS;
    };

    struct TaskOptions {
        std::string task_label = "TASK NUMBER ";
        std::string header = "STARTING TASKS";
        std::string footer = "END OF ALL TASKS\n";
        char exit_key = 'e';
        TimerConfig timer_cfg;
    };

    template <typename T>
    concept Streamable = requires(std::ostream & os, T value) {
        { os << value } -> std::convertible_to<std::ostream&>;
    };

    template<typename T>
    class PresentationSystem {
    public:
        explicit PresentationSystem(TaskOptions options = {})
            : m_Options(std::move(options)), m_Timer(m_Options.timer_cfg.resolution) {
        }

        void run(const std::vector<T>& tasks) {
            int task_count = 1;
            for (const auto& task : tasks) {
                if (!task) continue;

                clear_screen();

                print_header(task_count);

                execute_and_measure(task);

                if (should_exit(task_count)) break;
                task_count++;
            }
            finalize();
        }

    private:
        TaskOptions m_Options;
        Timer m_Timer;
        std::string Masured= "";

    private:
        void execute_and_measure(const T& task) {
            if (m_Options.timer_cfg.enabled) {
                m_Timer.start();
            }

            using ReturnType = std::invoke_result_t<T>;
            if constexpr (std::is_void_v<ReturnType>) {
                task();
            }
            else {
                auto result = task();
                if constexpr (Streamable<ReturnType>) {
                    std::cout << "\n\n[Return Value]: " << result << "\n";
                }
            }

            if (m_Options.timer_cfg.enabled) {
                Masured = std::format("[Execution Time]: {}", m_Timer.stop_and_get_unit_name());
            }
        }

        void print_header(int count) const {
            if (!m_Options.header.empty())
                std::cout << "=== " << m_Options.header << " ===\n";
            if (!m_Options.task_label.empty())
                std::cout << ">>> " << m_Options.task_label << count << ":\n";
        }

        bool should_exit(int count) const {
            std::cout << "\n------------------------------------";
            if (m_Options.timer_cfg.enabled) {
                std::cout << "\n" + Masured;
            }
            std::cout << "\nEnd of task " << count;
            std::cout << "\nPress Enter to continue, or '" << m_Options.exit_key << "' to skip: ";

            std::string input;
            std::getline(std::cin, input);
            return (!input.empty() && std::tolower(input[0]) == std::tolower(m_Options.exit_key));
        }

        void clear_screen() const {
            std::cout << "\033[2J\033[1;1H";
        }

        void finalize() const {
            clear_screen();
            std::cout << m_Options.footer;
        }
    };
}

namespace oop_lab_3 {

	/*struct person {
	private:
		uint64_t person_id;
	public:
		std::string name;
		uint64_t brith_year;
		uint64_t brith_month;
		uint64_t brith_day;

		std::string getDate() const {
			std::ostringstream s;
			s << brith_year << "-" << brith_month << "-" << brith_day;
			return s.str();
		}
	};*/

	struct student {
	private:
		uint64_t id{ 0 };
		std::string name{ "" };
	public :
	public:
		void setId(uint64_t new_id) {
			if (new_id > 9999) new_id = 9999;
			id = new_id;
		}
		void setName(std::string new_name) {
			name = new_name;
		}
		void display() {
			std::cout << "id  : " << id << "\n";
			std::cout << "name: " << name << "\n";
		}

		static void test() {
			student stu{};
			stu.setName("abdulhadi mohammad");
			stu.setId(19);
			stu.display();
		}
	};

	struct StudentGPA {
	private:
		double GPA;
	
	public :
		void setGPA(double new_gpa) {
			if (new_gpa > 4.0f)new_gpa = 0.0f;
			GPA = new_gpa;
		}
		
		double getGPA() {
			return GPA;
		}

		void display() {
			std::cout << "\nGPA: " << GPA << "\n";
		}

		static void test() {
			StudentGPA stu;
			stu.setGPA(5);
			std::cout << "\ntest getGPA func: " << stu.getGPA() << "\n";
			stu.display();
		}
	};

	struct Car {
	private:
		int speed = 0;
	public:
		void setSpeed(int new_speed) {
			if (new_speed < 0)new_speed = 0;
			if (new_speed > 200) new_speed = 200;

			speed = new_speed;
		}

		int getSpeed() {
			return speed;
		}

		void display() {
			std::cout << "\nspeed: " << speed << "\n";
		}

		static void test() {
			Car car;
			car.setSpeed(201);
			std::cout << "\ntest getSpeed func: " << car.getSpeed() << "\n";
			car.display();
		}
	};


	void run_tasks() {
		ps::TaskOptions taskOp{
			.task_label = "",
			.header = "",
			.footer = "",
			.timer_cfg {
				.enabled = true,
				.resolution = ps::enResolution::MICROSECONDS
			}
		};

		ps::PresentationSystem<std::function<void()>> tasks(taskOp);
		tasks.run({
			student::test,
			StudentGPA::test,
			Car::test
		});

	}
}


