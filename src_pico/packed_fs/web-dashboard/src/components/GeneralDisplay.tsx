import { ReactNode } from "react";

function GeneralDisplay({ children, ...props }: GeneralDisplayProps) {
    return (
        <div style={{ display: 'flex', padding: '12px', flexDirection: 'row', gap: '8px', width: '100%', justifyContent: 'space-around', alignItems: 'center' }}>
            {children}
        </div>
    );
}

type GeneralDisplayProps = {
    children?: ReactNode;
}

export default GeneralDisplay;