# Viva Questions and Answers – WebTrack

Use these questions and answers to prepare for your college viva.

---

### 1. What is the main objective of WebTrack?
**Answer:** WebTrack simulates how a web browser's **Back** and **Forward** buttons work using **two array-based stacks** in C, demonstrating the **LIFO (Last In, First Out)** principle alongside a separate browsing history log.

---

### 2. What is a stack, and what does LIFO mean?
**Answer:** A **stack** is a linear data structure where insertions (`push`) and deletions (`pop`) happen only at one end called the **top**. **LIFO** stands for **Last In, First Out**—the most recently added item is the first one removed, just like a stack of plates.

---

### 3. Why do we need TWO stacks instead of one stack or a single array?
**Answer:**
- The **Back Stack** keeps track of pages we visited *before* the current page.
- When we press **Back**, we move backward, so we must remember the page we just left in case the user presses **Forward** next.
- That is why the **Forward Stack** is needed: moving backward pushes onto `forwardStack`, and moving forward pops from `forwardStack` and pushes back onto `backStack`.

---

### 4. Why is there a separate `currentWebsite` variable?
**Answer:** Because the page you are currently viewing is neither in the Back list nor in the Forward list—it sits right between the two stacks. Keeping `currentWebsite` separate makes `push()` and `pop()` logic clean and easy to trace.

---

### 5. What happens when the user visits a new website?
**Answer:**
1. The URL is validated (`http://` or `https://`, non-empty, no spaces, within 255 chars).
2. If `currentWebsite` already holds a page, it is pushed onto `backStack` (after checking `isFull(&backStack)`).
3. `currentWebsite` is updated to the new URL.
4. `forwardStack` is cleared (`top = -1`).
5. The visit is added to `history` (if `history.count < 200`, else a warning is displayed).

---

### 6. Why must the Forward Stack be cleared when visiting a new website?
**Answer:** Suppose you visit `A -> B -> C`, go back to `B` (`Forward Stack = [C]`), and then open a brand-new site `D`. You have started a new browsing branch (`A -> B -> D`). In every real browser, `C` is no longer "forward" from `D`, so `forwardStack` must be cleared.

---

### 7. What happens during Go Back and Go Forward?
**Answer:**
- **Go Back:** If `backStack` is empty, display `"No previous website available."` Otherwise, push `currentWebsite` onto `forwardStack`, pop the top URL from `backStack`, and set it as `currentWebsite`.
- **Go Forward:** If `forwardStack` is empty, display `"No forward website available."` Otherwise, push `currentWebsite` onto `backStack`, pop the top URL from `forwardStack`, and set it as `currentWebsite`.

---

### 8. What is the difference between Browsing History and the two Navigation Stacks?
**Answer:**
- **Navigation Stacks (`backStack` and `forwardStack`)** manage short-term **Back/Forward movement** for the current session; items move between stacks and `forwardStack` gets cleared on new visits.
- **Browsing History (`BrowsingHistory`)** is a chronological **log** of every visited URL so the user can view all past visits, search them, or clear the log without breaking active Back/Forward navigation.

---

### 9. How do `isEmpty()`, `isFull()`, `push()`, `pop()`, and `clearStack()` work?
**Answer:**
- `isEmpty(s)`: Checks `s->top == -1`.
- `isFull(s)`: Checks `s->top == MAX_STACK_SIZE - 1` (`49` when size is `50`).
- `push(s, url)`: Checks `isFull(s)` first; if not full, increments `s->top` and copies `url` into `s->items[s->top]`.
- `pop(s, outUrl)`: Checks `isEmpty(s)` first; if not empty, copies `s->items[s->top]` into `outUrl` and decrements `s->top`.
- `clearStack(s)`: Sets `s->top = -1`, making the stack logically empty in O(1) time.

---

### 10. Why did you use `fgets()` instead of `gets()` or `scanf("%s", ...)`?
**Answer:**
- `gets()` was removed from C11 because it does not limit input length and causes **buffer overflows**.
- `scanf("%s", ...)` stops at spaces and leaves leftover newlines `\n` in the input buffer, which breaks subsequent prompts or causes infinite loops on invalid menu text.
- `fgets()` reads safely up to a fixed buffer size, and our `readLine()` helper strips `\n` and flushes extra characters if the user types too much.

---

### 11. How do you handle stack overflow, stack underflow, and the 200-entry history limit?
**Answer:**
- **Underflow:** Before `pop()`, `goBack()` checks `isEmpty(&backStack)` and `goForward()` checks `isEmpty(&forwardStack)`.
- **Overflow:** Before modifying `currentWebsite` or clearing stacks, `visitWebsite()`, `goBack()`, and `goForward()` check `isFull()` on the target stack so navigation state is never corrupted.
- **History limit:** When `history->count >= 200`, `addHistoryEntry()` prints a clear warning and preserves all 200 existing entries without overwriting them.

---

### 12. What is the time complexity of the stack and history operations?
**Answer:**
- `push()`, `pop()`, `isEmpty()`, `isFull()`, `clearStack()`: **$O(1)$** constant time (plus copying a fixed-size URL up to 255 characters).
- `displayStack()` and `viewBrowsingHistory()`: **$O(N)$** where $N$ is the number of stored items.
- `searchBrowsingHistory()`: **$O(N \times L)$** where $N$ is the number of history entries and $L$ is URL length.
