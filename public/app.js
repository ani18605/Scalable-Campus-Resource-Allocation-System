document.addEventListener('DOMContentLoaded', () => {
    const form = document.getElementById('builder-form');
    const select = document.getElementById('action-select');
    const dynamicFields = document.getElementById('dynamic-fields');
    const output = document.getElementById('console-output');

    const fieldsSchema = {
        'ADD_ROOM': ['Room ID (e.g. R101)', 'Block ID (e.g. C)', 'Floor', 'Capacity'],
        'ADD_CLASSROOM': ['Class ID', 'Building ID', 'Floor', 'Capacity'],
        'ALLOCATE_ROOMS': ['Student Count', 'Preferred Block', 'Min Capacity'],
        'MAINTENANCE': ['Scope Type (ROOM/FLOOR/BLOCK)', 'Scope ID (e.g. R101, C 3)', 'Status (ON/OFF)'],
        'QUERY_FREE_ROOMS': ['Block ID', 'Consecutive Count'],
        'REPORT': ['Scope Type (BLOCK/CAMPUS)', 'Scope ID (e.g. C, ALL)'],
        'BUILD_HOSTEL': ['Hostel Block ID (e.g. H1)', 'Number of Floors', 'Room Capacity'],
        'BUILD_CLASSROOM': ['Classroom Building ID (e.g. CB1)', 'Number of Floors', 'Classroom Capacity'],
        'BOOK_ANY_CLASSROOM': ['Building ID', 'Start Time (HH:MM)', 'End Time (HH:MM)', 'Min Seats Required']
    };

    function renderFields() {
        const action = select.value;
        dynamicFields.innerHTML = '';
        fieldsSchema[action].forEach((placeholder, idx) => {
            const wrapper = document.createElement('div');
            wrapper.style.display = 'flex';
            wrapper.style.flexDirection = 'column';
            wrapper.style.flex = '1 1 150px';

            const label = document.createElement('label');
            label.textContent = placeholder;
            label.style.fontSize = '0.8rem';
            label.style.color = 'var(--text-muted, #888)';
            label.style.marginBottom = '6px';

            const input = document.createElement('input');
            if (placeholder.includes('Time')) {
                input.type = 'time';
            } else {
                input.type = 'text';
            }
            input.placeholder = placeholder;
            input.required = true;
            input.dataset.idx = idx;
            
            wrapper.appendChild(label);
            wrapper.appendChild(input);
            dynamicFields.appendChild(wrapper);
        });
    }

    select.addEventListener('change', renderFields);
    renderFields();

    function appendLog(command, response) {
        const entry = document.createElement('div');
        entry.className = 'log-entry';
        
        const cmdEl = document.createElement('div');
        cmdEl.className = 'cmd';
        cmdEl.textContent = `> ${command}`;
        
        const resEl = document.createElement('div');
        resEl.style.whiteSpace = 'pre-wrap';
        resEl.textContent = response;

        entry.appendChild(cmdEl);
        entry.appendChild(resEl);
        
        output.appendChild(entry);
        output.scrollTop = output.scrollHeight;
    }

    let longClassTries = 0;

    function showToast(message) {
        let container = document.getElementById('toast-container');
        if (!container) {
            container = document.createElement('div');
            container.id = 'toast-container';
            document.body.appendChild(container);
        }
        const toast = document.createElement('div');
        toast.className = 'toast';
        toast.textContent = message;
        container.appendChild(toast);
        setTimeout(() => {
            toast.style.opacity = '0';
            setTimeout(() => toast.remove(), 500);
        }, 3000);
    }

    async function executeCommand(cmd, silent = false) {
        if (!cmd.trim()) return;
        
        try {
            const res = await fetch('/api/command', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ command: cmd })
            });
            const data = await res.json();
            
            if (!silent) {
                if (data.error) {
                    appendLog(cmd, `ERROR: ${data.error}`);
                } else {
                    appendLog(cmd, data.output);
                }
            }
        } catch (err) {
            if (!silent) appendLog(cmd, `NETWORK ERROR: ${err.message}`);
        }
    }

    form.addEventListener('submit', async (e) => {
        e.preventDefault();
        const action = select.value;
        const inputs = Array.from(dynamicFields.querySelectorAll('input'));
        const args = inputs.map(i => i.value.trim()).join(' ');

        if (action === 'BOOK_ANY_CLASSROOM') {
            const startParts = inputs[1].value.split(':');
            const endParts = inputs[2].value.split(':');
            if (startParts.length === 2 && endParts.length === 2) {
                const startMins = parseInt(startParts[0]) * 60 + parseInt(startParts[1]);
                const endMins = parseInt(endParts[0]) * 60 + parseInt(endParts[1]);
                const duration = endMins - startMins;

                if (duration >= 240) {
                    longClassTries++;
                    if (longClassTries === 1) {
                        showToast(`Think about the students. Attention spans have limits.`);
                        appendLog('SYSTEM', `Blocked: Think about the students.`);
                        return; // Stop execution
                    } else if (longClassTries === 2) {
                        showToast(`Are you sure this is necessary?`);
                        appendLog('SYSTEM', `Blocked: Are you sure this is necessary?`);
                        return; // Stop execution
                    } else {
                        showToast(`fine. we lost. you win.`);
                        longClassTries = 0;
                    }
                }
            }
            const cmd = `${action} ${args}`;
            await executeCommand(cmd);
        } else if (action === 'BUILD_HOSTEL') {
            const block = inputs[0].value.trim();
            const floors = parseInt(inputs[1].value);
            const cap = parseInt(inputs[2].value);
            const roomsPerFloor = 100;
            
            appendLog(`BUILD_HOSTEL`, `Generating Hostel Block ${block} with ${floors} floors (100 rooms/floor)...`);
            
            for(let f = 1; f <= floors; f++) {
                for(let r = 1; r <= roomsPerFloor; r++) {
                    const rId = `${block}-${f}${r.toString().padStart(2, '0')}`;
                    const cmd = `ADD_ROOM ${rId} ${block} ${f} ${cap}`;
                    await executeCommand(cmd, true);
                }
            }
            appendLog('SYSTEM', `Hostel Block ${block} fully constructed!`);
        } else if (action === 'BUILD_CLASSROOM') {
            const building = inputs[0].value.trim();
            const floors = parseInt(inputs[1].value);
            const cap = parseInt(inputs[2].value);
            const classesPerFloor = 15;
            
            appendLog(`BUILD_CLASSROOM`, `Generating Classroom Building ${building} with ${floors} floors (15 rooms/floor)...`);
            
            for(let f = 1; f <= floors; f++) {
                for(let c = 1; c <= classesPerFloor; c++) {
                    const cId = `CL_${building}-${f}${c.toString().padStart(2, '0')}`;
                    const cmd = `ADD_CLASSROOM ${cId} ${building} ${f} ${cap}`;
                    await executeCommand(cmd, true);
                }
            }
            appendLog('SYSTEM', `Classroom Building ${building} fully constructed!`);
        } else {
            const cmd = `${action} ${args}`;
            await executeCommand(cmd);
        }
    });
});
