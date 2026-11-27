# Test Cases – WebTrack Browser History Management System

All test cases below were executed against `.\webtrack.exe` compiled with:

```powershell
gcc -std=c99 -Wall -Wextra -Wpedantic webtrack.c -o webtrack
```

---

## Functional & Edge-Case Test Matrix

| TC# | Scenario | Input / Action | Expected Output | Status |
|---|---|---|---|---|
| 1 | Start the program | Launch `.\webtrack.exe` | Welcome banner and menu `0`–`10` displayed; `currentWebsite` empty | Pass |
| 2 | Visit first website | Choice `1`, URL `https://www.google.com` | `[Success] Now visiting: https://www.google.com`; Back Stack stays empty | Pass |
| 3 | Visit second website | Choice `1`, URL `https://www.github.com` | `google.com` pushed to Back Stack; `currentWebsite` becomes `github.com` | Pass |
| 4 | Visit multiple websites | Visit `wikipedia.org`, then `http://example.com` | Back Stack holds 3 URLs (`wikipedia.org` at TOP); `currentWebsite` is `example.com` | Pass |
| 5 | Go back once | Choice `2` from `example.com` | `example.com` pushed to Forward Stack; `currentWebsite` becomes `wikipedia.org` | Pass |
| 6 | Go back multiple times | Choice `2` again from `wikipedia.org` | `wikipedia.org` pushed to Forward Stack; `currentWebsite` becomes `github.com` | Pass |
| 7 | Go forward once | Choice `3` from `github.com` | `github.com` pushed to Back Stack; `currentWebsite` becomes `wikipedia.org` | Pass |
| 8 | Go forward multiple times | Choice `3` again from `wikipedia.org` | `wikipedia.org` pushed to Back Stack; `currentWebsite` becomes `example.com` | Pass |
| 9 | Go back then visit new website | Go back to `github.com`, then visit `https://www.stackoverflow.com` | Forward Stack is cleared; info message printed; Option `6` shows empty Forward Stack | Pass |
| 10 | Go back when Back Stack empty | Choice `2` at program start | Displays `No previous website available.` | Pass |
| 11 | Go forward when Forward Stack empty | Choice `3` at start or at latest page | Displays `No forward website available.` | Pass |
| 12 | Display current website | Choice `4` before and after visiting | Displays `No website is currently open...` initially; prints URL after visit | Pass |
| 13 | Display Back Stack | Choice `5` | Displays items from `[TOP]` down to `[1]` in LIFO order | Pass |
| 14 | Display Forward Stack | Choice `6` | Displays items from `[TOP]` down to `[1]`, or `Forward Stack is empty.` | Pass |
| 15 | Display browsing history | Choice `7` | Lists all visited URLs in chronological order (`1.` to `N.`) | Pass |
| 16 | Search existing website | Choice `8`, query `GITHUB` | Case-insensitive match finds `https://www.github.com` (`Total matches found: 1`) | Pass |
| 17 | Search non-existent website | Choice `8`, query `nonexistent.xyz` | Displays `No matching website found in browsing history.` | Pass |
| 18 | Clear browsing history | Choice `10` -> `n` (cancel), then Choice `10` -> `y` (confirm) | `n` cancels safely; `y` clears history to 0 entries while keeping active stacks intact | Pass |
| 19 | Invalid menu choices | Enter `abc`, `99`, `-1`, and blank line | Displays `[Error] Invalid menu choice. Please enter a number from 0 to 10.` without crashing | Pass |
| 20 | Back Stack capacity limit | Visit 51 websites (`50` in Back Stack + `1` current), then visit 52nd | Displays `[Error] Back Stack is full (limit: 50 URLs)` and preserves `site51` and 50/50 stack | Pass |

---

## Additional Input Validation & History Limit Tests

| TC# | Scenario | Input / Action | Expected Output | Status |
|---|---|---|---|---|
| 21 | Empty URL | Choice `1`, press Enter | `[Error] Website URL cannot be empty.` | Pass |
| 22 | Missing protocol | Choice `1`, `google.com` | `[Error] Invalid URL! URL must start with http:// or https://` | Pass |
| 23 | Incomplete URL | Choice `1`, `https://` | `[Error] Incomplete URL! Please include a website name after the protocol.` | Pass |
| 24 | URL with spaces | Choice `1`, `https://my site.com` | `[Error] Website URL cannot contain spaces.` | Pass |
| 25 | URL exceeding 255 characters | Choice `1`, `https://` + 255 `'a'`s | `[Error] Website URL exceeds maximum allowed length (255 characters).` | Pass |
| 26 | History 200-entry limit | Perform 225 visits across back-and-visit cycles | First 200 stored; visits 201–225 show `[Warning] Browsing history is full (200/200 entries)` without overwriting old entries | Pass |
