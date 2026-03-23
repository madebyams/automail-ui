import { TestBed } from '@angular/core/testing';
import { SendersView } from './senders';

describe('SendersView', () => {
  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [SendersView],
    }).compileComponents();
  });

  it('should create', () => {
    const fixture = TestBed.createComponent(SendersView);
    expect(fixture.componentInstance).toBeTruthy();
  });

  it('deleteStatus starts as idle', () => {
    const fixture = TestBed.createComponent(SendersView);
    expect(fixture.componentInstance.deleteStatus()).toBe('idle');
  });

  it('addSender() trims whitespace and calls window function', () => {
    const fixture = TestBed.createComponent(SendersView);
    const comp = fixture.componentInstance;
    (window as any).addToSenderList = vi.fn().mockResolvedValue(['test@example.com']);
    comp.email = '  test@example.com  ';
    comp.addSender();
    expect((window as any).addToSenderList).toHaveBeenCalledWith('test@example.com');
    expect(comp.email).toBe('');
    delete (window as any).addToSenderList;
  });

  it('addSender() does not add a sender that already exists', () => {
    const fixture = TestBed.createComponent(SendersView);
    const comp = fixture.componentInstance;
    (window as any).addToSenderList = vi.fn();
    comp.model.senderList = ['existing@example.com'];
    comp.email = 'existing@example.com';
    comp.addSender();
    expect((window as any).addToSenderList).not.toHaveBeenCalled();
    delete (window as any).addToSenderList;
  });

  it('addSender() does nothing for blank input', () => {
    const fixture = TestBed.createComponent(SendersView);
    const comp = fixture.componentInstance;
    (window as any).addToSenderList = vi.fn();
    comp.email = '   ';
    comp.addSender();
    expect((window as any).addToSenderList).not.toHaveBeenCalled();
    delete (window as any).addToSenderList;
  });

  it('dismissBanner() resets deleteStatus to idle', () => {
    const fixture = TestBed.createComponent(SendersView);
    const comp = fixture.componentInstance;
    comp['deleteStatus'].set('success');
    comp.dismissBanner();
    expect(comp.deleteStatus()).toBe('idle');
  });

  it('processAndDelete() sets status to success and clears list on resolution', async () => {
    const fixture = TestBed.createComponent(SendersView);
    const comp = fixture.componentInstance;
    comp.model.senderList = ['a@example.com'];
    (window as any).doDelete = vi.fn().mockResolvedValue(undefined);
    await comp.processAndDelete();
    expect(comp.deleteStatus()).toBe('success');
    expect(comp.model.senderList).toEqual([]);
    delete (window as any).doDelete;
  });

  it('processAndDelete() sets status to error on rejection', async () => {
    const fixture = TestBed.createComponent(SendersView);
    const comp = fixture.componentInstance;
    (window as any).doDelete = vi.fn().mockRejectedValue(new Error('server error'));
    await comp.processAndDelete();
    expect(comp.deleteStatus()).toBe('error');
    delete (window as any).doDelete;
  });
});
