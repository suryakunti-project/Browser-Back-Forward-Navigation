# Algorithms – WebTrack Browser History Management System

This document explains the step-by-step algorithms implemented in `webtrack.c`.

---

## Data Structures Used

- **`backStack`**: Array-based stack with `top` initialized to `-1` (capacity: `50`).
- **`forwardStack`**: Array-based stack with `top` initialized to `-1` (capacity: `50`).
- **`currentWebsite`**: String holding the active URL (`""` initially).
- **`history`**: Array of up to `200` visited URLs with `count` initialized to `0`.

---

## 1. Algorithm: Visit a New Website

**Input:** A URL string `newUrl` entered by the user.  
**Output:** Updates `currentWebsite`, `backStack`, `forwardStack`, `history`, and `stats`.

### Steps:
1. Start.
2. Prompt the user to enter `newUrl` using `fgets()` safely (check for input overflow or EOF).
3. Trim leading and trailing whitespace from `newUrl`.
4. Validate `newUrl`:
   - If `newUrl` is empty, display an error and return.
   - If `newUrl` contains any spaces, display an error and return.
   - If `newUrl` does not start with `http://` or `https://`, display an error and return.
   - If nothing follows the protocol prefix, display an error and return.
5. Check whether `currentWebsite` is non-empty:
   - If `currentWebsite` is non-empty and `isFull(backStack)` is true:
     - Display a Back Stack overflow error without changing `currentWebsite` or `forwardStack`, and return.
   - If `currentWebsite` is non-empty and `backStack` is not full:
     - Call `push(backStack, currentWebsite)`.
6. Copy `newUrl` into `currentWebsite`.
7. Call `clearStack(forwardStack)` so old forward pages are discarded.
8. Increment `stats.totalVisits` by `1`.
9. Append `currentWebsite` to `history`:
   - If `history.count >= 200`, display a capacity warning and do not overwrite old entries.
   - Otherwise, store `currentWebsite` at `history.urls[history.count]` and increment `history.count`.
10. Display a success message showing `currentWebsite`.
11. Stop.

---

## 2. Algorithm: Go Back

**Input:** User selects menu option `2`.  
**Output:** Moves to the previous website if available.

### Steps:
1. Start.
2. Check if `isEmpty(backStack)` is true:
   - If true, display `"No previous website available."` and return.
3. Check if `isFull(forwardStack)` is true:
   - If true, display a Forward Stack full error without modifying navigation state, and return.
4. Call `push(forwardStack, currentWebsite)` to save the current page for future Forward navigation.
5. Call `pop(backStack, previousUrl)` to retrieve the most recently visited previous page.
6. Copy `previousUrl` into `currentWebsite`.
7. Increment `stats.backCount` by `1`.
8. Display the new `currentWebsite`.
9. Stop.

---

## 3. Algorithm: Go Forward

**Input:** User selects menu option `3`.  
**Output:** Moves forward to the next website if available.

### Steps:
1. Start.
2. Check if `isEmpty(forwardStack)` is true:
   - If true, display `"No forward website available."` and return.
3. Check if `isFull(backStack)` is true:
   - If true, display a Back Stack full error without modifying navigation state, and return.
4. Call `push(backStack, currentWebsite)` to save the current page onto the Back Stack.
5. Call `pop(forwardStack, nextUrl)` to retrieve the top URL from the Forward Stack.
6. Copy `nextUrl` into `currentWebsite`.
7. Increment `stats.forwardCount` by `1`.
8. Display the new `currentWebsite`.
9. Stop.

---

## 4. Algorithm: Display Browsing History

**Input:** `history` structure containing `urls[]` and `count`.  
**Output:** Prints all recorded visits in chronological order.

### Steps:
1. Start.
2. Check if `history.count == 0`:
   - If true, display `"Browsing history is empty."` and return.
3. Display the total stored count (`history.count / 200`).
4. Set loop counter `i = 0`.
5. Repeat while `i < history.count`:
   - Print `i + 1` and `history.urls[i]`.
   - Increment `i` by `1`.
6. Stop.

---

## 5. Algorithm: Search Browsing History

**Input:** A search keyword or URL `query` entered by the user.  
**Output:** Displays all matching history entries and total match count.

### Steps:
1. Start.
2. Check if `history.count == 0`:
   - If true, display `"Browsing history is empty. Nothing to search."` and return.
3. Read `query` safely using `fgets()` and trim leading/trailing spaces.
4. If `query` is empty or exceeds maximum length, display an error and return.
5. Initialize `matches = 0` and `i = 0`.
6. Repeat while `i < history.count`:
   - Compare `history.urls[i]` with `query` using case-insensitive substring matching (`containsIgnoreCase()`).
   - If a match is found:
     - Print `i + 1` and `history.urls[i]`.
     - Increment `matches` by `1`.
   - Increment `i` by `1`.
7. If `matches == 0`:
   - Display `"No matching website found in browsing history."`
   Else:
   - Display `Total matches found: matches`.
8. Stop.
