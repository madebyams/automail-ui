import { TestBed } from '@angular/core/testing';
import { Header } from './header';
import { ThemeService } from '../services/theme.service';

describe('Header', () => {
  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [Header],
    }).compileComponents();
  });

  it('should create', () => {
    const fixture = TestBed.createComponent(Header);
    expect(fixture.componentInstance).toBeTruthy();
  });

  it('exposes the shared ThemeService instance', () => {
    const fixture = TestBed.createComponent(Header);
    const themeService = TestBed.inject(ThemeService);
    expect(fixture.componentInstance['theme']).toBe(themeService);
  });
});
