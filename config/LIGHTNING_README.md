# ⚡ Lightning Bolt Animation for ZMK

This custom ZMK configuration adds a lightning bolt animation that appears from a cloud when you type at high speeds on your nice!nano v2 with Corne keyboard.

## Features

- **Cloud Display**: A persistent cloud appears in the top-right corner of the OLED display
- **Lightning Animation**: Lightning bolts shoot down from the cloud when typing speed exceeds the threshold
- **WPM-Based Triggering**: Lightning frequency increases with typing speed
- **Smooth Animation**: 150ms lightning animation with zigzag pattern
- **Non-Intrusive**: Works alongside existing status widgets (WPM, layer, output, peripheral status)

## How It Works

1. **WPM Monitoring**: The system monitors your Words Per Minute (WPM) in real-time
2. **Threshold Triggering**: Lightning activates when WPM exceeds 30 (configurable)
3. **Adaptive Frequency**: At high WPM (>80), the threshold adjusts to maintain frequent lightning
4. **Animation Sequence**: Lightning bolts animate for 150ms with a zigzag pattern

## Configuration

### WPM Threshold
- **Default**: 30 WPM
- **High Speed**: Threshold adjusts to (current WPM - 10) when WPM > 80
- **Location**: `lightning_display.c` line 18

### Animation Duration
- **Default**: 150ms
- **Location**: `lightning_display.c` line 67

### Cloud Position
- **Default**: Top-right corner (x: 103, y: 2)
- **Location**: `lightning_display.c` line 60

## Customization

### Change WPM Threshold
```c
static uint8_t wpm_threshold = 30; // Change this value
```

### Adjust Animation Speed
```c
if (elapsed < 150) { // Change 150 to desired duration in milliseconds
```

### Modify Cloud Position
```c
draw_cloud(buffer, DISPLAY_WIDTH - 25, 2); // Change coordinates
```

### Customize Lightning Pattern
Edit the `draw_lightning()` function in `lightning_display.c` to change the zigzag pattern.

## Files

- `lightning_display.c` - Main animation implementation
- `CMakeLists.txt` - Build configuration
- `corne.overlay` - Devicetree configuration
- `corne.conf` - ZMK configuration

## Building

The lightning animation is automatically included when you build your ZMK firmware:

```bash
west build -d build/left -b nice_nano_v2 -- -DSHIELD=corne_left
west build -d build/right -b nice_nano_v2 -- -DSHIELD=corne_right
```

## Troubleshooting

### Lightning Not Appearing
1. Check that WPM is above the threshold (30 by default)
2. Verify display is enabled in `corne.conf`
3. Check build logs for any compilation errors

### Animation Too Fast/Slow
1. Adjust the animation duration in `lightning_display.c`
2. Modify the WPM threshold for different trigger frequencies

### Display Issues
1. Ensure `CONFIG_ZMK_DISPLAY=y` is set in `corne.conf`
2. Check that the nice-view-mod is properly configured in `west.yml`

## Performance

- **Memory Usage**: Minimal - only a few static variables
- **CPU Impact**: Negligible - simple pixel drawing operations
- **Battery Impact**: None - uses existing display refresh cycle

## Future Enhancements

Potential improvements:
- Multiple lightning bolts simultaneously
- Different lightning patterns based on WPM ranges
- Sound effects (if supported by hardware)
- Color variations (for RGB displays)
- Thunder cloud variations

---

*Enjoy your electrifying typing experience! ⚡* 
