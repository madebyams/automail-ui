import { AppModel, Config } from './app.model';

declare global {
    interface Window {
        doDelete:        (emailList: AppModel) => Promise<void>;
        hasConfig:       ()                    => Promise<boolean>;
        getSenders:      ()                    => Promise<string[]>;
        saveImapConfig:  (config: Config)      => Promise<string>;
        loadImapConfig:  ()                    => Promise<Config | null>;
        addToSenderList:      (sender: string) => Promise<string[]>;
        removeFromSenderList: (sender: string) => Promise<string[]>;
    }
}

export const isNative = (): boolean => typeof window.doDelete === 'function';
