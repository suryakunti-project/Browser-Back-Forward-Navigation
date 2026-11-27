# Flowcharts – WebTrack Browser History Management System

This document contains Mermaid flowcharts for the main program loop and the three core two-stack navigation operations.

---

## 1. Main Program Flowchart

```mermaid
flowchart TD
    A["Start Program"] --> B["Initialize backStack, forwardStack, history, stats, currentWebsite"]
    B --> C["Display Welcome Banner"]
    C --> D["Display Menu (0 to 10)"]
    D --> E["Read Menu Choice Safely with fgets()"]
    E --> F{"Valid choice 0-10?"}
    F -- "No" --> G["Display Invalid Choice Error"]
    G --> D
    F -- "Yes" --> H{"Choice == 0?"}
    H -- "Yes" --> I["Display Goodbye Message & Exit"]
    H -- "No" --> J["Execute Selected Option (1-10)"]
    J --> D
```

---

## 2. Visit a New Website Flowchart

```mermaid
flowchart TD
    A["Start visitWebsite()"] --> B["Read URL using fgets()"]
    B --> C{"Input length <= 255?"}
    C -- "No" --> D["Show URL Too Long Error & Return"]
    C -- "Yes" --> E["Trim Spaces & Validate URL"]
    E --> F{"Starts with http:// or https:// and non-empty?"}
    F -- "No" --> G["Show Invalid URL Error & Return"]
    F -- "Yes" --> H{"Is currentWebsite non-empty?"}
    H -- "No" --> K["Set currentWebsite = newUrl"]
    H -- "Yes" --> I{"Is backStack full?"}
    I -- "Yes" --> J["Show Back Stack Full Error & Return Safely"]
    I -- "No" --> L["push(backStack, currentWebsite)"]
    L --> K
    K --> M["clearStack(forwardStack)"]
    M --> N["Increment totalVisits"]
    N --> O{"Is history full (200)?"}
    O -- "Yes" --> P["Show History Full Warning (Keep Old Entries)"]
    O -- "No" --> Q["Add currentWebsite to history"]
    P --> R["Display Success Message & Return"]
    Q --> R
```

---

## 3. Go Back Flowchart

```mermaid
flowchart TD
    A["Start goBack()"] --> B{"Is backStack empty?"}
    B -- "Yes" --> C["Display 'No previous website available.' & Return"]
    B -- "No" --> D{"Is forwardStack full?"}
    D -- "Yes" --> E["Display Forward Stack Full Error & Return"]
    D -- "No" --> F["push(forwardStack, currentWebsite)"]
    F --> G["pop(backStack, previousUrl)"]
    G --> H["Set currentWebsite = previousUrl"]
    H --> I["Increment backCount"]
    I --> J["Display New Current Website & Return"]
```

---

## 4. Go Forward Flowchart

```mermaid
flowchart TD
    A["Start goForward()"] --> B{"Is forwardStack empty?"}
    B -- "Yes" --> C["Display 'No forward website available.' & Return"]
    B -- "No" --> D{"Is backStack full?"}
    D -- "Yes" --> E["Display Back Stack Full Error & Return"]
    D -- "No" --> F["push(backStack, currentWebsite)"]
    F --> G["pop(forwardStack, nextUrl)"]
    G --> H["Set currentWebsite = nextUrl"]
    H --> I["Increment forwardCount"]
    I --> J["Display New Current Website & Return"]
```
