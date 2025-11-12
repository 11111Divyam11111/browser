 Browser Rendering Engine

A lightweight, custom-built browser rendering engine written in C++ that parses HTML and CSS files and renders them using SFML graphics library. This project demonstrates low-level understanding of browser internals by implementing a complete parsing and rendering pipeline from scratch.

 🚀 Features

- Custom HTML Parser: Stack-based DOM tree construction without external browser dependencies
- CSS Styling Engine: From-scratch implementation supporting selectors and style properties
- SFML Rendering: Visual rendering of styled HTML content with interactive viewport navigation
- Text Wrapping: Automatic text wrapping based on viewport width
- Interactive Viewport: Keyboard-controlled scrolling and navigation
- Style Application: Dynamic styling with support for colors, font sizes, and text decorations

 🏗️ Architecture

 Core Components

1. HTML Parser (`html.cpp`, `html.h`)
   - Parses raw HTML into a DOM tree structure
   - Stack-based tag matching and tree construction
   - Text content extraction and whitespace handling

2. CSS Parser (`css.cpp`, `css.h`)
   - Tokenizes and parses CSS rules
   - Supports multiple selector types (Universal, Tag, Class, ID)
   - Declaration parsing with property-value pairs
   - Value type detection (Color, Unit, Numeric, Keyword)

3. Rendering Engine (`main.cpp`)
   - Traverses DOM tree and applies styles
   - Calculates layout and text positioning
   - Handles text wrapping based on viewport width
   - Renders styled content to SFML window

4. Helper Utilities (`helpers.cpp`, `helpers.h`)
   - File I/O operations
   - Stream to string conversion

 📋 Supported CSS Features

 Selectors
- Universal selector (``)
- Tag selectors (`h1`, `h2`, `h3`, `h4`, `h5`, `p`, `div`)

 Properties
- `color`: Hex color values (`RRGGBB` )
- `font-size`: Pixel units (`px`)
- `text-decoration`: `underline`, `line-through`

 Value Types Detected
- Color: Hex format (`000000`, `fff`)
- Unit: Numbers with units (`15px`, `2em`, `50%`)
- Numeric: Plain numbers
- Keyword/String: Text values

 🎮 Controls

| Key | Action |
|-----|--------|
| `↑` | Scroll up |
| `↓` | Scroll down |
| `←` | Scroll left |
| `→` | Scroll right |
| `Backspace` | Reset viewport to top |
| `Escape` | Close window |

 🛠️ Technical Implementation

 DOM Tree Construction
```
HTML Input → Tokenization → Stack-based Parsing → DOM Tree
```

The parser uses a stack to maintain the hierarchy of HTML elements, pushing opening tags and popping them when encountering closing tags, automatically building parent-child relationships.

 CSS Rule Application
```
CSS Input → Selector Parsing → Declaration Parsing → Style Map → Render Tree
```

Styles are stored in an unordered map keyed by selector values, allowing O(1) lookup during rendering.

 Rendering Pipeline
```
DOM Tree + Style Map → Layout Calculation → Text Wrapping → SFML Drawing
```

The engine traverses the DOM tree, applies matching styles, calculates positions with text wrapping, and renders to the SFML window.

 📊 Performance

- HTML Parsing: Measured execution time displayed on startup
- CSS Parsing: Measured execution time displayed on startup
- Rendering: Real-time at 60 FPS with SFML
- Memory: Efficient tree structure with proper cleanup via `deleteTree()`

 🔧 Build & Run

```bash
 Compile
g++ -std=c++17 main.cpp src/html.cpp src/css.cpp src/helpers.cpp \
    -I./headers -lsfml-graphics -lsfml-window -lsfml-system -o browser_engine

 Run
./browser_engine file.html style.css
```

 Requirements
- C++17 or higher
- SFML 2.5+
- DejaVu Serif font (or modify font path in `main.cpp`)

 📄 Sample HTML Structure

```html
<html>
<body>
    <h1>Resume</h1>
    <div1>
        <h3>Browser Rendering Engine</h3>
        <p>Built a lightweight HTML/CSS parser and renderer...</p>
        <p>Implemented a CSS styling engine from scratch...</p>
    </div1>
    <div2>
        <h3>Video Call Application</h3>
        <p>Built a real-time video calling platform...</p>
    </div2>
</body>
</html>
```

 🎨 Sample CSS Rules

```css

*{
    color: 0f12a5;
    font-size: 15px;
}


h1 {
    text-decoration: underline;
    font-size: 40px;
    color: 000000;
}

h3 {
    font-size: 34px;
    color: 000000;
}
```

 🧪 Parsing Examples

 CSS Comment Handling
```css
/ This is a comment /
h1 { color: 000000; }
```

 Multiple Declarations
```css
h1 {
    font-size: 40px;
    color: 000000;
    text-decoration: underline;
}
```

 Universal Selector
```css
 *{
    font-size: 15px;
    color: 0f12a5;
}
```

 🚧 Limitations

- No support for CSS classes/IDs
- Limited CSS property support (color, font-size, text-decoration)
- No inline styles or style attribute parsing
- No CSS cascade/specificity calculation
- No box model implementation (padding, margin, border)
- No layout algorithms (flexbox, grid)
- Images limited to single sprite for navigation cursor

 🎯 Key Achievements

✅ Zero external browser engine dependencies  
✅ Complete HTML/CSS parsing from scratch  
✅ Custom DOM tree data structure  
✅ Style matching and application system  
✅ Interactive viewport with scrolling  
✅ Text wrapping algorithm  
✅ Memory-safe tree cleanup  
✅ Performance timing instrumentation  

 📚 Learning Outcomes

This project demonstrates understanding of:
- Lexical analysis and tokenization
- Recursive descent parsing
- Tree data structures and traversal
- Stack-based algorithms
- Graphics rendering fundamentals
- Event-driven programming
- Memory management in C++
- Performance optimization

 🔮 Future Enhancements

- [ ] Support for more CSS properties (margin, padding, display)
- [ ] JavaScript interpreter integration
- [ ] Network layer for fetching remote resources
- [ ] Support for images and multimedia
- [ ] Layout algorithms (flex, grid)
- [ ] CSS specificity and cascade implementation
- [ ] Support for inline styles
- [ ] Better error handling and reporting

---

Built as a demonstration of browser internals and low-level systems programming  
Language: C++17  
Graphics: SFML 2.5+  
License: Educational/Portfolio Project
