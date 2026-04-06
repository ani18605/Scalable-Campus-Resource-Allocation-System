🏫 Scalable Campus Resource Allocation System










🚀 Executive Summary

The Scalable Campus Resource Allocation System is a high-performance, full-stack solution designed to efficiently manage large-scale campus infrastructure such as hostels and classrooms.

Built with a hybrid architecture combining C++ and Node.js, this system leverages advanced data structures and low-level optimizations to deliver:

⚡ Ultra-fast allocation and querying
🧠 Algorithmically efficient scheduling
🏢 Physically-aware clustering of resources
🌐 Real-time interaction through a sleek terminal-style UI

This project demonstrates how systems programming (C++) and web technologies (Node.js + JS) can be combined to build production-grade, scalable infrastructure tools.

🧩 Architecture Overview
🔧 Tech Stack
Layer	Technology
Backend	C++17 (compiled native executable)
Middleware	Node.js (Express + child_process.spawn)
Frontend	Vanilla JavaScript, HTML5, CSS3
⚙️ System Design
Frontend (Terminal UI)
        ↓ (HTTP / Fetch API)
Node.js Express Server
        ↓ (IPC via STDIN/STDOUT)
C++ Core Engine (Executable)
        ↓
Advanced Data Structures (Segment Trees, Interval Trees)
🧠 Core Engine (C++ Integration)

The heart of the system is a C++17 compiled executable, responsible for all critical computations:

🚀 Runs as a native high-performance engine
🔁 Communicates with Node.js using standard input/output streams
⚡ Ensures deterministic execution unaffected by network latency
🧮 Executes complex allocation logic in O(log N) time
📊 Algorithms Used
🔹 Segment Trees
Used for hostel room allocation
Efficiently finds consecutive empty room blocks
Time Complexity: O(log N)
🔹 Interval Trees with Lazy Propagation
Handles classroom booking schedules
Prevents time overlap conflicts
Supports dynamic updates and queries
🔹 Physical Clustering Logic
Rooms are sorted and allocated based on floor proximity
Ensures groups are placed as physically close as possible
Eliminates randomness caused by asynchronous processing delays
✨ Core Features
🏢 Infrastructure Generation
BUILD_HOSTEL
Dynamically generates multi-floor hostel structures
Scales efficiently (e.g., 10 floors × 75 rooms in milliseconds)
BUILD_CLASSROOM
Creates academic blocks with similar scalability
🛏️ Smart Allocation
ALLOCATE_ROOMS
Assigns student groups into consecutive clustered rooms
Optimized using segment trees
🕒 Intelligent Scheduling
BOOK_ANY_CLASSROOM
Accepts flexible time ranges (e.g., 08:00 - 14:00)
Uses interval trees to avoid overlapping bookings
📊 Real-Time Analytics
QUERY_FREE_ROOMS / REPORT
Provides live status:
FREE
ALLOCATED
MAINTENANCE
🛠️ Maintenance Handling
MAINTENANCE
Marks rooms/floors as unavailable
Dynamically updates allocation structures
🛡️ Robust Edge Case Handling
❌ Backend Safeguards

Throws strict errors:

THIS DOESNT EXISTS

when querying undefined structures

⚠️ Frontend Fatigue Checkers
Prevents unrealistic scheduling inputs
Uses dynamic toast notifications
Example:
Blocks excessively long class durations
💻 Terminal-Style UI
🌙 Dark mode interface
🖥️ Command-line inspired design
⚡ Minimal, fast, distraction-free interaction
🔽 Dropdown-triggered operations mapped to backend commands
🛠️ Getting Started
📦 Installation
npm install
⚙️ Compile C++ Engine
g++ -std=c++17 -Wall -Wextra -O2 *.cpp -o build_campus.exe
▶️ Run the Server
node server.js
🌐 Access the Application

Open your browser and navigate to:

http://localhost:3000
📈 Performance Highlights
⚡ Millisecond-level infrastructure generation
🧠 Logarithmic-time allocation and querying
🔄 Efficient IPC between Node.js and C++
🏗️ Scales to massive campus datasets
🔮 Future Enhancements
📊 Visualization dashboards (heatmaps, occupancy graphs)
🧠 AI-based predictive allocation
🌍 Multi-campus distributed support
🔐 Role-based admin authentication
📱 Mobile-responsive UI
🤝 Contributing

Contributions are welcome! Feel free to:

Fork the repo
Create a feature branch
Submit a pull request
📄 License

This project is licensed under the MIT License.

💡 Final Note

This project is a strong demonstration of:

Systems-level optimization using C++
Real-time orchestration with Node.js
Clean UI interaction using Vanilla JS

It bridges the gap between algorithmic rigor and practical deployment, making it ideal for high-scale infrastructure systems.
