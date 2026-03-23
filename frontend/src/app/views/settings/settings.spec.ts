import { TestBed } from '@angular/core/testing';
import { ClientType } from '../../app.model';
import { SettingsView } from './settings';

describe('SettingsView', () => {
  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [SettingsView],
    }).compileComponents();
  });

  it('should create', () => {
    const fixture = TestBed.createComponent(SettingsView);
    expect(fixture.componentInstance).toBeTruthy();
  });

  it('saveStatus starts as idle', () => {
    const fixture = TestBed.createComponent(SettingsView);
    expect(fixture.componentInstance.saveStatus()).toBe('idle');
  });

  it('isImap() returns true when client_type is IMAP', () => {
    const fixture = TestBed.createComponent(SettingsView);
    const comp = fixture.componentInstance;
    comp.imap.client_type = ClientType.IMAP;
    expect(comp.isImap()).toBe(true);
  });

  it('isImap() returns false when client_type is COM', () => {
    const fixture = TestBed.createComponent(SettingsView);
    const comp = fixture.componentInstance;
    comp.imap.client_type = ClientType.COM;
    expect(comp.isImap()).toBe(false);
  });

  it('onSslToggle() changes port 143 → 993 when ssl is enabled', () => {
    const fixture = TestBed.createComponent(SettingsView);
    const comp = fixture.componentInstance;
    comp.imap.ssl = true;
    comp.imap.port = 143;
    comp.onSslToggle();
    expect(comp.imap.port).toBe(993);
  });

  it('onSslToggle() changes port 993 → 143 when ssl is disabled', () => {
    const fixture = TestBed.createComponent(SettingsView);
    const comp = fixture.componentInstance;
    comp.imap.ssl = false;
    comp.imap.port = 993;
    comp.onSslToggle();
    expect(comp.imap.port).toBe(143);
  });

  it('onSslToggle() leaves non-standard port unchanged', () => {
    const fixture = TestBed.createComponent(SettingsView);
    const comp = fixture.componentInstance;
    comp.imap.ssl = true;
    comp.imap.port = 8993;
    comp.onSslToggle();
    expect(comp.imap.port).toBe(8993);
  });

  it('saveImap() sets status to saved when window function resolves', async () => {
    const fixture = TestBed.createComponent(SettingsView);
    const comp = fixture.componentInstance;
    (window as any).saveImapConfig = vi.fn().mockResolvedValue(undefined);
    await comp.saveImap();
    expect(comp.saveStatus()).toBe('saved');
    delete (window as any).saveImapConfig;
  });

  it('saveImap() sets status to error when window function rejects', async () => {
    const fixture = TestBed.createComponent(SettingsView);
    const comp = fixture.componentInstance;
    (window as any).saveImapConfig = vi.fn().mockRejectedValue(new Error('network error'));
    await comp.saveImap();
    expect(comp.saveStatus()).toBe('error');
    delete (window as any).saveImapConfig;
  });
});
