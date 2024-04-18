import { ReactNode } from "react";

/**
 * Verticle list of variables.
 */
function DisplayPanel({ children, ...props }: DisplayPanelProps) {
    return (
        <div style={{ display: 'flex', padding:'12px', flexDirection: 'column', gap:'4px', justifyContent: 'space-around', alignItems: 'center', width: '95%' }}>
            {children}
        </div>
    );
}

type DisplayPanelProps = {
    children?: ReactNode;
}

export default DisplayPanel;