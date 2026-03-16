import { AppModel, Config } from './app.model';

declare global {
    interface Window {
        doDelete:        (emailList: AppModel) => Promise<void>;
        hasConfig:       ()                    => Promise<boolean>;
        saveImapConfig:  (config: Config)      => Promise<string>;
        loadImapConfig:  ()                    => Promise<Config | null>;
    }
}

export const isNative = (): boolean => typeof window.doDelete === 'function';
