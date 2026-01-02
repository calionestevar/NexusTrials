# NexusTrials - Portfolio Overview

**10-year QA automation veteran bringing enterprise testing practices to game development.**

## The Ask
Show game studios that professional-grade test frameworks and AI validation can solve gaming's biggest quality challenges—just like they do in healthcare, agtech, and marine systems.

## What You're Looking At

### The Game
A lightweight Unreal Engine 5.7 project with **3 playable variants** that intentionally keeps gameplay simple so the **testing framework is the star**.

### The Framework Integration
- **[NexusQA](https://github.com/calionestevar/NexusQA)**: Automated test orchestration and distributed tracing
- **Distributed Testing**: Multi-server load testing and chaos injection
- **AI Validation**: Behavior verification and anomaly detection (OrionAI coming soon)
- **Compliance**: Medical/automotive-grade testing patterns adapted for games

## Why This Matters

| Background | Applied To Games |
|-----------|-----------------|
| **Cerner (5 yrs)** - DICOM/medical imaging validation | → Game server/backend testing |
| **AGI SureTrack (2 yrs)** - Cypress automation (agtech) | → Game client/UI automation |
| **Garmin (2+ yrs)** - Network resilience (marine) | → Game network chaos engineering |

## Quick Stats

✅ **Clean UE 5.7 build** - All modules compiling, linking, running  
✅ **Framework integrated** - NexusQA submodule properly configured  
✅ **Professional setup** - CI/CD ready with headless testing support  
✅ **Portfolio-grade code** - Real game systems (health, AI, physics) not toy examples  

## Implementation Results (Testing Phase)

1. ✅ Project builds cleanly with NexusQA integrated
2. ✅ **19 automated tests executing successfully**
3. ✅ **FNexusTestContext pattern implemented** - Enterprise test design
   - All GAMETHREAD tests refactored with Context parameter
   - Automatic actor cleanup via RAII destructors
   - Safe world validation before execution
4. ✅ **Performance metrics integrated** - ArgusLens + ObserverNetworkDashboard
   - NEXUS_PERF_TEST macro for performance validation
   - Automatic FPS/memory/hitch monitoring
5. ✅ **Test discovery & distributed tracing** - Full reporting infrastructure
   - Tests auto-register via static constructors
   - Fail-fast on critical test failures
   - Results logged to ObserverNetworkDashboard

## For Hiring Managers

**Looking for someone to build/improve your game testing infrastructure?**

This project demonstrates:

✅ **Enterprise QA expertise applied to games**
- 10 years professional test automation (medical imaging, agtech, marine → games)
- Framework architecture patterns that scale
- Real architectural decisions with documented tradeoffs

✅ **Hands-on refactoring & improvement**
- Took existing framework and improved test patterns
- FNexusTestContext: RAII cleanup for better test isolation
- ArgusLens integration: Performance metrics in test infrastructure
- Not toy examples—real architectural depth

✅ **Technical depth demonstrated**
- Separation of concerns (parallel vs. game-thread tests)
- Soft dependencies via reflection (no hard compile-time coupling)
- RAII patterns in modern C++
- Distributed test execution & fail-fast strategies

**Want to discuss?** Let's talk about building world-class testing infrastructure for your studio.

---

**Full documentation**: See [README.md](README.md) for complete project details, setup instructions, and framework documentation.

**Frameworks & Integration**:
- **[NexusQA](https://github.com/calionestevar/NexusQA)** - Test automation framework with distributed tracing, performance metrics, and chaos engineering
- **OrionAI** - AI validation system (coming soon)
