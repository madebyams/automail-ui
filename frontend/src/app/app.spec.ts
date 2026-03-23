import { TestBed } from '@angular/core/testing';
import { App } from './app';

describe('App', () => {
  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [App],
    }).compileComponents();
  });

  it('should create the app', () => {
    const fixture = TestBed.createComponent(App);
    expect(fixture.componentInstance).toBeTruthy();
  });

  it('defaults to senders view', () => {
    const fixture = TestBed.createComponent(App);
    expect(fixture.componentInstance['view']()).toBe('senders');
  });

  it('switches to settings view when hasConfig returns false', async () => {
    (window as any).hasConfig = vi.fn().mockResolvedValue(false);
    const fixture = TestBed.createComponent(App);
    fixture.detectChanges();
    await fixture.whenStable();
    expect(fixture.componentInstance['view']()).toBe('settings');
    delete (window as any).hasConfig;
  });

  it('stays on senders view when hasConfig returns true', async () => {
    (window as any).hasConfig = vi.fn().mockResolvedValue(true);
    const fixture = TestBed.createComponent(App);
    fixture.detectChanges();
    await fixture.whenStable();
    expect(fixture.componentInstance['view']()).toBe('senders');
    delete (window as any).hasConfig;
  });
});
