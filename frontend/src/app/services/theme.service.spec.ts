import { ThemeService } from './theme.service';

describe('ThemeService', () => {
  let service: ThemeService;

  beforeEach(() => {
    localStorage.clear();
    document.documentElement.removeAttribute('data-theme');
    service = new ThemeService();
  });

  it('initialises with dark theme by default', () => {
    service.init();
    expect(service.isDark()).toBe(true);
    expect(document.documentElement.getAttribute('data-theme')).toBe('dark');
    expect(localStorage.getItem('theme')).toBe('dark');
  });

  it('reads saved light theme from localStorage on init', () => {
    localStorage.setItem('theme', 'light');
    service.init();
    expect(service.isDark()).toBe(false);
    expect(document.documentElement.getAttribute('data-theme')).toBe('light');
  });

  it('reads saved dark theme from localStorage on init', () => {
    localStorage.setItem('theme', 'dark');
    service.init();
    expect(service.isDark()).toBe(true);
    expect(document.documentElement.getAttribute('data-theme')).toBe('dark');
  });

  it('toggle() switches from dark to light', () => {
    service.init();
    service.toggle();
    expect(service.isDark()).toBe(false);
    expect(localStorage.getItem('theme')).toBe('light');
    expect(document.documentElement.getAttribute('data-theme')).toBe('light');
  });

  it('toggle() switches from light to dark', () => {
    localStorage.setItem('theme', 'light');
    service.init();
    service.toggle();
    expect(service.isDark()).toBe(true);
    expect(localStorage.getItem('theme')).toBe('dark');
    expect(document.documentElement.getAttribute('data-theme')).toBe('dark');
  });

  it('toggle() twice returns to original theme', () => {
    service.init();
    service.toggle();
    service.toggle();
    expect(service.isDark()).toBe(true);
  });
});
