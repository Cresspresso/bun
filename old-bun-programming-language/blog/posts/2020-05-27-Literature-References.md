
[Previous](./2020-05-06-Writing-a-Paper.md) • [All Posts](../posts.md) • ~~Next~~

27 May 2020

Elijah Shadbolt

# Literature References

## References I have viewed recently:

- A Complete Guide to Programming in C++
    - http://www.lmpt.univ-tours.fr/~volkov/C++.pdf

- The Design of C++
    - https://youtu.be/69edOm889V4
    - Bjarne Stroustrup
    - March 2nd, 1994
    - University Video Communications
    - Site accessed 12 May 2020
    - Notes:
        - Important features of C++.
        - C with classes, never intended to replace C with newfangled syntax (good ol' for loops).
        - Emphasis on concepts: how we think about the code as if it represents objects.
        - Object oriented practices.
        - Virtual functions.
        - Templates.

- Practical C++ Modules
    - https://youtu.be/szHV6RdQdg8
    - Boris Kolpackov
    - CppCon 2019 at Aurora
    - Site accessed 13 May 2020
    - Notes:
        - ODR one definition rule
        - How to use modules
        - Use modules for single platform projects.
        - Don't use modules for cross-platform projects.
        - Modules does not allow cyclic module imports.

- Gregoire M, Kleper S, Solter N. Professional C++. Vol 2nd ed. Hoboken, N.J.: Wrox; 2011. http://search.ebscohost.com/login.aspx?direct=true&db=e000xww&AN=452868&site=ehost-live. Accessed May 5, 2020.
    - Very in-depth book

- C++ Code Smells - Jason Turner
    - https://youtu.be/nqfgOCU_Do4

- OOP Is Dead, Long Live Data-oriented Design
    - https://youtu.be/yy8jQgmhbAU

- Bob Nystrom - Is There More to Game Architecture than ECS?
    - https://youtu.be/JxI3Eu5DPwE

- Clean Coders Hate What Happens to Your Code When You Use These Enterprise Programming Tricks
    - https://youtu.be/FyCYva9DhsI
    - Import statements, make them broad for less code noise, not fragile, not individual classes.

- "How To Design A Good API and Why it Matters"
    - Bloch, J. (2007).
    - https://youtu.be/aAb7hSCtvGw
    - Google Tech Talks
    - Notes:
        - "When in doubt, leave it out."
            - "you can always add later, but you can never remove"
        - "Don't make the client do anything the module could do"
        - "Don't violate the Principle of Least Astonishment"
    - All Notes:
        - About APIs
        - All programs are modular
        - Composable, reusable, sensible
        - API design is like Language Design except APIs are constrained by the language it is written in.
        - They create a tool for programmers to communicate intent to computer and other programmers.
        - Platform consists of language and its libraries.
        - Good API:
            - easy to learn
            - easy to use (even without docs)
            - hard to misuse
            - easy to read and maintain code which uses it
            - sufficiently powerful to satisfy requirements
            - easy to evolve
            - appropriate to audience
        - Outline of slideshow
            - Process of API design
            - General Principles
            - Class design
            - Method design
            - Exception design
            - Refactoring API design
        - Gather Requirements
            - If they propose solutions, extract the problem
            - Open mind
        - Short Spec (one page long)
            - Agility trumps completeness
            - Get feedback from as many people as possible
            - Short spec means easy to modify
            - Flesh it out as you gain confidence
        - Write the API interface before implementing
            - Start before even specified it properly
            - Continue writing the API as you flesh it out
            - Examples and unit tests, the code lives on
                - Good examples copied by users means they use it well
        - service provider interface (SPI)
            - middleware, wrapper for plug-ins
            - Testing on one plug-in couples it rigidly, hard to add more
            - Testing on two plug-ins is probably ok
            - Testing on three servies makes it easy to add more
        - Don't over-constrain the API
            - "Aim to displease everyone equally"
            - Still have a lead direction to head towards
        - Expect to make mistakes
            - Real-world use will show the flaws
            - API will evolve
            - Solution is never perfect
            - Some things are fixed with this new approach
        - API should do one thing and do it well
            - Easy to explain.
            - Good names. Hard to name indicates bad design.
            - Good names drive good designs.
            - Modular, split, hide details
        - API should be as small as possible but no smaller
            - Satisfy its requirements
            - "When in doubt, leave it out."
            - New things can be added, but things can never be removed.
            - Conceptual weight (ease of understanding) more important than bulk
        - Implementation should not impact API
            - Details may be confusing to users
            - Inhibit freedom to change impl
            - Do not overspecify behaviour of methods (e.g. hash functions, different impls should be able to return different ints)
            - All tuning parameters are suspect
            - Don't let implementation details leak into the API
                - Stop exception propagation, wrap it in an exception thrown by the API
                - Don't say "this form implements serializable"
        - Class invariants; hide fields as private
        - Minimize coupling
        - Names Matter, API is a Little Language
            - names should be largely self-explanatory
                - no abbreviations
            - Consistent
                - same word means same thing
                - remove? delete? why are there two words in the api?
            - Be regular, strive for symmetry
            - Good code reads like prose
                - `if (car.speed() > 2 * MAX_CAR_SPEED) { car.honk_horn(); }`
        - Documentation Matters
            - "Reuse is something far easier to say than to do. Doing it requires both good design and very good documentation. Even when we see good design, which is still infrequently, we won't see the components reused without good documentation." - D. L. Parnas, Software Aging. Proceedings of the 16th International Conference on Software Engineering. 1994
            - Document EVERYTHING
                - Class: what it represents
                - Interface
                - Parameter: units, form, ownership
                - Function/Method: contract with client, preconditions about other state, postconditions, side effects
                - Constructor
                - Exception
            - Document state space very carefully (mutable changes)
                - remember: separate implementation from API
        - Consider performance consequences of API design decisions
            - Bad API decisions can limit performance
                - mutable
                - constructor instead of static factory
                - not using interface, returning implementation class (cannot change)
            - Do not warp API to gain performance
                - Headaches forever is not worth the fixed underlying performance issue
                - Good design usually coincides with good performance
            - Bad design example: return dynamically allocated Vector2 mutable class from getSize, instead of reference to immutable Vector2 instance
        - API must coexist peacefully with platform
            - Do what is customary
                - Standard naming conventions
                - Avoid obsolete parameter and return types
                - Mimic patterns in core APIs and other languages
            - Take advantage of API-friendly language features
                - generics, varargs, enums, default arguments
            - Know and avoid API traps and pitfalls
                - Finalizers, public static const arrays
            - Don't transliterate (literal translation loses the meaning in different language)
        - Classes should be immutable unless for good reason
            - If mutable keep state space small and well defined
                - Make it clear when it's legal to call a mutable method
            - Short lifetimes means less confusing
        - Subclass only when it makes sense
            - Liskov Substitution
            - Every Set is-a Collection
            - Do not subclass for ease of implementation
                - Every Set is-a Vector makes no sense
        - Inheritance: Design and Document or else Prohibit it
            - Inheritance violates encapsulation
                - Subclass sensitive to impl details of superclass
            - If inheritance allowed, document self-use
                - How do methods use one another?
            - Conservative policy: all concrete classes final
        - "Don't make the client do anything the module could do"
            - Reduce boilerplate
        - "Don't violate the Principle of Least Astonishment"
            - Users of the API should not be surprised by the behaviour
            - Worth extra implementation effort
            - Worth reduced performance
            - Name things after their most important purpose/function, not second purpose or by-products.
        - Fail Fast - report errors asap
            - Compile time is best - static checks
            - At runtime, complain at the first moment of the user doing something wrong
            - Bad Example: `table.put(object, object)` then 10min later `save()` throws error because one of the values is not a string.
        - Provide programmatic access to all data available in string form
            - Otherwise the client will parse the string themselves
                - Painful for clients
                - Worse: Turns string format into de facto API
            - Bad example: exceptions only able to be printed, cannot get line number
        - Overload with care
            - Avoid ambiguous overloads
                - Conservative: no two with same number of arguments
            - Just because you can doesn't mean you should
                - remember "you can always add later, but you can never remove"
                - Often better to give it another name
        - Use appropriate parameter and return types
            - favour interface types over classes for input
            - use most specific possible input parameter type
            - don't use string if a better type exists
            - don't use floating for money
            - double rather than float (negligible performance difference)
        - Consistent parameter ordering across methods
        - Avoid long parameter lists
        - Avoid return values that demand exceptional processing
            - Rarely returns null? users might forget.
                - return empty container, not null

## References that I have found and have access to:

- Stroustrup, B. "What Should We Teach New Software Developers? Why?"
    - https://cacm.acm.org/magazines/2010/1/55760-what-should-we-teach-new-software-developers-why/fulltext
    - https://dl.acm.org/doi/pdf/10.1145/1629175.1629192
- "Modules the beginner's guide" - Daniela Engert - Meeting C++ 2019
    - https://youtu.be/Kqo-jIq4V3I
- "Modules are coming" - Bryce Adelstein Lelbach - Meeting Cpp 2019
    - https://youtu.be/yee9i2rUF3s
- CppCoreGuidelines
    - github.com/isocpp/CppCoreGuidelines
- C++ Developer Survey "Lite": 2018-02
    - https://isocpp.org/files/papers/CppDevSurvey-2018-02-summary.pdf

## References suggested to me which I have access to:

- Lahav, Ori, et al. "Repairing sequential consistency in C/C++ 11." ACM SIGPLAN Notices 52.6 (2017): 618-632.
    - https://dl.acm.org/doi/pdf/10.1145/3140587.3062352
    - Relevance: 

## Collections:

- http://www.stroustrup.com/index.html
- https://www.youtube.com/user/CppCon
- https://www.youtube.com/channel/UCJpMLydEYA08vusDkq3FmjQ
- https://isocpp.org/
- https://herbsutter.com/gotw/

## References suggested to me which I do not have access to:

- Masters, Timothy. Assessing and Improving Prediction and Classification: Theory and Algorithms in C++. Apress, 2017.
    - $83
- Duffy, Daniel J. "Introduction to the Boost C++ Libraries Volume III–Algorithms, Concurrency, Generic Programming and Higher-order Functions, Numerics." (2018).
    - $97
- Lakos, John. Large-Scale C++ Volume I: Process and Architecture. Addison-Wesley Professional, 2019.
    - $90
- Deitel, Paul, and Harvey Deitel. "C How To Program: With An Introduction To C++, \[Paperback] Paul Deitel And Harvey Deitel." (2018).

## Interesting Facts

- Dennis Ritchie was one of the creators of Unix C.
