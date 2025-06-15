Thank you for your interest in contributing to VersaNES. To ensure the continued quality, efficiency, and extensibility of the project, please follow these core principles when proposing changes:

1. Prioritize Performance
Optimizations that reduce CPU cycles, memory usage, or I/O overhead are highly encouraged. Efficiency should take precedence over abstraction unless abstraction directly supports extensibility or maintainability. Avoid introducing performance regressions without clear justification.

2. Reduce Footprint
Lean code is a core objective. Contributions that shrink binary size, reduce memory consumption, or eliminate unnecessary assets are strongly preferred. Favor minimalism where possible, provided functionality remains intact.

3. Minimize Dependencies
Avoid introducing new external libraries unless they offer substantial and demonstrable value. Each dependency must be evaluated for impact on portability, maintainability, and build complexity. Simplicity is paramount.

4. Avoid Redundancy
Functionality already achievable through external ROMs, plug-ins, or scripting should not be replicated in core code. Only include such features if they are essential to VersaNES’s mission or significantly enhance extensibility.

5. Favor Extensibility
Design enhancements to be modular and extensible. New functionality should be exposed through flexible interfaces rather than hardcoded as built-in features. The goal is to allow others to build upon VersaNES without altering its core.

6. Avoid Unnecessary Machine Accuracy
Do not pursue perfect hardware emulation accuracy unless it is essential. VersaNES embraces both legacy NES behavior and creative enhancements, so prioritize practical and efficient solutions. Any advanced or speculative accuracy improvements should be optional, clearly modular, and must not compromise performance or the project’s core principles.

7. Ensure Clear and Consistent Documentation
All contributions must be accompanied by clear, concise documentation that explains the purpose, usage, and limitations of new features or changes. This includes inline code comments, updated README sections, and any necessary configuration or usage instructions. Well-documented code improves maintainability and helps new contributors understand the project quickly.

8. Prioritize Debian-Based Platform Compatibility
Changes should be designed and tested primarily for Debian-based systems. Support or optimizations targeting other operating systems are considered unnecessary.